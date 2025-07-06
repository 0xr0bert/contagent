package sh.greener.contagent.lib

import com.github.luben.zstd.ZstdOutputStream
import kotlinx.serialization.ExperimentalSerializationApi
import kotlinx.serialization.json.Json
import kotlinx.serialization.json.JsonNamingStrategy
import kotlinx.serialization.json.encodeToStream

class Runner(val configuration: Configuration) {
    private fun perceiveBeliefs(time: Int) {
        configuration.agents.forEach { it.updateActivationsForAllBeliefs(time, configuration.beliefs) }
    }

    private fun performActions(time: Int) {
        configuration.agents.forEach { it.performAction(
            time, configuration.behaviours, configuration.beliefs, configuration.rng
        ) }
    }

    private fun tick(time: Int) {
        configuration.logger.info { "[time=$time] Perceiving beliefs" }
        perceiveBeliefs(time)
        configuration.logger.info { "[time=$time] Performing actions" }
        performActions(time)
    }

    private fun tickBetween(startTime: Int, endTime: Int) {
        for (i in startTime until endTime) {
            tick(i)
        }
    }

    fun run() {
        configuration.logger.info {
            "Starting simulation {\"start\": ${configuration.startTime}, \"end\": ${configuration.endTime}, " +
                    "\"nBeliefs\": ${configuration.beliefs.size}, \"nBehaviours\": ${configuration.behaviours.size}, " +
                    "\"nAgents\": ${configuration.agents.size}}"
        }
        performActions(configuration.startTime - 1)
        tickBetween(configuration.startTime, configuration.endTime)
        configuration.logger.info { "Simulation complete; serializing output" }

        if (configuration.fullOutput) {
            serializeAndOutputFull()
        } else {
            serializeAndOutputSummary()
        }
        configuration.logger.info { "Output successfully serialized" }
    }

    @OptIn(ExperimentalSerializationApi::class)
    private fun serializeAndOutputFull() {
        val agentSpecs = configuration.agents.map { it.toAgentSpec() }
        val json = Json {
            namingStrategy = JsonNamingStrategy.SnakeCase
        }
        configuration.outputStream.use { ZstdOutputStream(it).use {
            zstdOS -> json.encodeToStream(agentSpecs, zstdOS)
        } }
    }

    @OptIn(ExperimentalSerializationApi::class)
    private fun serializeAndOutputSummary() {
        val data = (configuration.startTime until configuration.endTime).map {
            calculateSummaryStats(configuration, it)
        }.map { it.toSummarySpec() }

        val json = Json {
            namingStrategy = JsonNamingStrategy.SnakeCase
        }

        configuration.outputStream.use { ZstdOutputStream(it).use {
            zstdOS -> json.encodeToStream(data, zstdOS)
        }}
    }
}
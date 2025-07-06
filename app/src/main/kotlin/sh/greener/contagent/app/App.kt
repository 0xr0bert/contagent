@file:OptIn(ExperimentalSerializationApi::class)

package sh.greener.contagent.app

import com.github.luben.zstd.ZstdInputStream
import io.github.oshai.kotlinlogging.KLogger
import io.github.oshai.kotlinlogging.KotlinLogging
import kotlinx.serialization.ExperimentalSerializationApi
import kotlinx.serialization.json.Json
import kotlinx.serialization.json.JsonNamingStrategy
import kotlinx.serialization.json.decodeFromStream
import sh.greener.contagent.lib.Agent
import sh.greener.contagent.lib.Behaviour
import sh.greener.contagent.lib.Belief
import sh.greener.contagent.lib.Configuration
import sh.greener.contagent.lib.Runner
import sh.greener.contagent.lib.json.AgentSpec
import sh.greener.contagent.lib.json.BehaviourSpec
import sh.greener.contagent.lib.json.BeliefSpec
import java.io.FileInputStream
import java.io.FileOutputStream
import kotlin.random.Random
import kotlin.system.exitProcess

fun main(args: Array<String>) {
    val logger = KotlinLogging.logger("Contagent")
    if (args.size < 6 || args.size > 8) {
        logger.error { "Incorrect number of args. Correct usage is:\n"+
            "./contagent <start-time> <end-time> <agents> <beliefs> <behaviours> <output> <full-output> <seed>\n" +
            "for example\n" +
            "./contagent 1 1000 agents.json.zst beliefs.json behaviours.json output.json.zst false 123"
        }
        exitProcess(1)
    }

    val startTime: Int = args[0].toInt()
    val endTime: Int = args[1].toInt()

    if (endTime <= startTime) {
        throw IllegalArgumentException("End time must be after start time");
    }

    logger.info { "Loading behaviours" }
    val behaviours = loadBehaviours(args[4], logger)
    logger.info { "Loading beliefs" }
    val beliefs = loadBeliefs(args[3], behaviours, logger)
    logger.info { "Loading agents" }
    val agents = loadAgents(args[2], behaviours, beliefs, logger)
    val output = FileOutputStream(args[5])
    val seed = if (args.size >= 8) {
        args[7].toLong()
    } else {
        System.currentTimeMillis()
    }

    val fullOutput = args.size >= 7 && args[6] == "true"
    val configuration = Configuration(behaviours, beliefs, agents, startTime, endTime, output, fullOutput, Random(seed), logger)
    val runner = Runner(configuration)
    runner.run()
}

private fun loadBehaviours(filePath: String, logger: KLogger): List<Behaviour> {
    try {
        val input = FileInputStream(filePath)
        val json = Json {
            namingStrategy = JsonNamingStrategy.SnakeCase
        }
        val specs = json.decodeFromStream<List<BehaviourSpec>>(input)
        return specs.map { Behaviour(it) }
    } catch (e: Exception) {
        logger.error(e) { "Error reading behaviours JSON" }
        throw e
    }
}

private fun loadBeliefs(filePath: String, behaviours: List<Behaviour>, logger: KLogger): List<Belief> {
    try {
        val input = FileInputStream(filePath)
        val json = Json {
            namingStrategy = JsonNamingStrategy.SnakeCase
        }
        val specs = json.decodeFromStream<List<BeliefSpec>>(input)
        val behavioursByUuid = behaviours.associateBy { it.uuid.toString() }
        val beliefs = specs.map { Belief(it, behavioursByUuid) }
        val beliefsByUuid = beliefs.associateBy { it.uuid.toString() }
        specs.forEach { linkBeliefs(it, beliefsByUuid) }
        return beliefs
    } catch (e: Exception) {
        logger.error(e) { "Error reading beliefs JSON" }
        throw e
    }
}

private fun linkBeliefs(spec: BeliefSpec, beliefsByUuid: Map<String, Belief>) {
    val belief = beliefsByUuid[spec.uuid]!!
    spec.relationships.forEach {
        belief.relationships[beliefsByUuid[it.key]!!] = it.value
    }
}

private fun loadAgents(filePath: String, behaviours: List<Behaviour>, beliefs: List<Belief>, logger: KLogger): List<Agent> {
    try {
        logger.info { "Parsing agents JSON" }
        val input = FileInputStream(filePath)
        val zstdInput = ZstdInputStream(input)
        val json = Json {
            namingStrategy = JsonNamingStrategy.SnakeCase
        }
        val specs = json.decodeFromStream<List<AgentSpec>>(zstdInput)
        val behavioursByUuid = behaviours.associateBy { it.uuid.toString() }
        val beliefsByUuid = beliefs.associateBy { it.uuid.toString() }
        logger.info { "Transforming agents JSON to unlinked agents" }
        val agents = specs.map { Agent(it, behavioursByUuid, beliefsByUuid) }
        logger.info { "Link agents" }
        val agentsByUuid = agents.associateBy { it.uuid.toString() }
        specs.forEach { linkAgents(it, agentsByUuid) }

        // Process any agents with missing activation
        beliefs.forEach { belief ->
            agents.forEach { agent ->
                agent.activations.forEach { acts ->
                    if (!acts.contains(belief)) {
                        acts.put(belief, 0.0)
                    }
                }
            }
        }
        return agents
    } catch (e: Exception) {
        logger.error(e) { "Error reading agents JSON" }
        throw e
    }
}

private fun linkAgents(spec: AgentSpec, agentsByUuid: Map<String, Agent>) {
    val agent = agentsByUuid[spec.uuid]!!
    spec.friends.forEach {
        agent.friends[agentsByUuid[it.key]!!] = it.value
    }
}
package sh.greener.contagent.lib

import sh.greener.contagent.lib.json.AgentSpec
import java.util.UUID
import kotlin.collections.sumOf
import kotlin.math.max
import kotlin.math.min
import kotlin.random.Random

class Agent(
    override val uuid: UUID, val activations: MutableList<MutableMap<Belief, Double>>,
    val friends: MutableMap<Agent, Double>, val actions: MutableList<Behaviour>, val deltas: Map<Belief, Double>,
    val performanceRelationships: Map<Belief, Map<Behaviour, Double>>
) : UUIDd {
    constructor(spec: AgentSpec, behavioursByUuid: Map<String, Behaviour>, beliefsByUuid: Map<String, Belief>): this(
        UUID.fromString(spec.uuid),
        ArrayList(spec.activations.map { elem -> HashMap(elem.mapKeys { beliefsByUuid[it.key]!! }) }),
        HashMap<Agent, Double>(),
        ArrayList(spec.actions.map { behavioursByUuid[it]!! }),
        spec.deltas.mapKeys { beliefsByUuid[it.key]!! },
        HashMap(
            spec.performanceRelationships
                .map { Pair(beliefsByUuid[it.key]!!, HashMap(it.value.mapKeys { behaviourUuid -> behavioursByUuid[behaviourUuid.key]!! })) }
                .toMap()
        )
    )


    private fun weightedRelationship(simTime: Int, b1: Belief, b2: Belief): Double {
        val acts = activations[simTime]
        if (acts.contains(b1)) {
            val relationships = b1.relationships
            if (relationships.contains(b2)) {
                return (acts[b1]?: 0.0) * (relationships[b2]?: 0.0)
            }
        }
        return 0.0
    }

    private fun contextualize(simTime: Int, b: Belief, beliefs: Collection<Belief>): Double {
        val size = beliefs.size;
        if (size == 0) {
            return 0.0
        } else {
            val context = beliefs.sumOf { weightedRelationship(simTime, b, it) }
            return context / size
        }
    }

    private fun getActionsOfFriends(simTime: Int): Map<Behaviour, Double> {
        val map = HashMap<Behaviour, Double>()
        for (entry in friends) {
            val action = entry.key.actions[simTime]
            map.put(action, map.getOrDefault(action, 0.0) + entry.value)
        }
        return map
    }

    private fun pressure(b: Belief, actionsOfFriends: Map<Behaviour, Double>): Double {
        val size = friends.size

        if (size == 0) {
            return 0.0
        } else {
            val pressure = actionsOfFriends
                .map { b.perceptions[it.key]?.times(it.value) ?: 0.0 }
                .sum()
            return pressure / size
        }
    }

    private fun activationChange(
        simTime: Int, belief: Belief, beliefs: Collection<Belief>, actionsOfFriends: Map<Behaviour, Double>
    ): Double {
        val pressure = pressure(belief, actionsOfFriends)

        return if (pressure > 0.0) {
            (1.0 + contextualize(simTime, belief, beliefs)) / 2.0 * pressure
        } else {
            (1.0 - contextualize(simTime, belief, beliefs)) / 2.0 * pressure
        }
    }

    private fun updateActivation(
        simTime: Int, belief: Belief, beliefs: Collection<Belief>, actionsOfFriends: Map<Behaviour, Double>
    ) {
        val delta = deltas[belief]!!
        val activations = activations[simTime - 1]
        val activation = activations[belief]?: 0.0

        val activationChangeV = activationChange(simTime - 1, belief, beliefs, actionsOfFriends)

        val newActivation = max(-1.0, min(1.0, delta * activation + activationChangeV))

        if (this.activations.size <= simTime) {
            this.activations.add(simTime, HashMap())
        }
        this.activations[simTime][belief] = newActivation
    }

    fun updateActivationsForAllBeliefs(simTime: Int, beliefs: Collection<Belief>) {
        val actionsOfFriends = getActionsOfFriends(simTime)
        beliefs
            .forEach { updateActivation(simTime, it, beliefs, actionsOfFriends) }
    }

    fun performAction(simTime: Int, behaviours: Collection<Behaviour>, beliefs: Collection<Belief>, rng: Random) {
        val unnormalizedProbabilities: List<Pair<Behaviour, Double>> = behaviours.map { behaviour ->
            Pair(behaviour, beliefs
                .sumOf { belief ->
                    performanceRelationships[belief]!![behaviour]!! * (activations[simTime][belief]?: 0.0)
                })
        }

        val sorted = unnormalizedProbabilities.sortedBy { (_, v) -> v }
        val last = unnormalizedProbabilities.last()

        if (last.second < 0.0) {
            actions.add(simTime, last.first)
        } else {
            val filteredProbs = sorted.filter { it.second >= 0.0 }
            if (filteredProbs.size == 1) {
                actions.add(simTime, filteredProbs.first().first)
            } else {
                val normalizingFactor = filteredProbs.sumOf { it.second }
                val normalizedProbs = filteredProbs.map { Pair(it.first, it.second / normalizingFactor)}
                var chosenBehaviour = normalizedProbs.last().first
                var rv = rng.nextDouble()
                for (pair in normalizedProbs) {
                    rv -= pair.second
                    if (rv <= 0.0) {
                        chosenBehaviour = pair.first
                        break
                    }
                }

                actions.add(simTime, chosenBehaviour)
            }
        }
    }

    fun toAgentSpec(): AgentSpec {
        return AgentSpec(
            uuid.toString(),
            actions.map { it.uuid.toString() },
            activations.map { m -> m.mapKeysTo(HashMap()) { it.key.uuid.toString() } },
            deltas.mapKeysTo(HashMap()) { it.key.uuid.toString() },
            friends.mapKeysTo(HashMap()) { it.key.uuid.toString() },
            performanceRelationships.map { entry ->
                entry.key.uuid.toString() to entry.value.mapKeysTo(HashMap()) {
                    it.key.uuid.toString()
                }
            }.toMap()
        )
    }

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as Agent

        return uuid == other.uuid
    }

    override fun hashCode(): Int {
        return uuid.hashCode()
    }


}
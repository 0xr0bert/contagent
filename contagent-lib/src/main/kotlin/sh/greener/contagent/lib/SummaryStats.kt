package sh.greener.contagent.lib

import sh.greener.contagent.lib.json.SummarySpec

class SummaryStats(
    val meanActivations: Map<Belief, Double>,
    val sdActivations: Map<Belief, Double>,
    val medianActivations: Map<Belief, Double>,
    val nonzeroActivations: Map<Belief, Int>,
    val nPerformers: Map<Behaviour, Int>
) {
    fun toSummarySpec(): SummarySpec {
        return SummarySpec(
            meanActivations.mapKeys { it.key.uuid.toString() },
            sdActivations.mapKeys { it.key.uuid.toString() },
            medianActivations.mapKeys { it.key.uuid.toString() },
            nonzeroActivations.mapKeys { it.key.uuid.toString() },
            nPerformers.mapKeys { it.key.uuid.toString() }
        )
    }
}
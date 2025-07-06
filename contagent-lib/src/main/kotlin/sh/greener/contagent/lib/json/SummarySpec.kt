package sh.greener.contagent.lib.json

import kotlinx.serialization.Serializable

@Serializable
data class SummarySpec(
    val meanActivations: Map<String, Double>,
    val sdActivations: Map<String, Double>,
    val medianActivations: Map<String, Double>,
    val nonzeroActivations: Map<String, Int>,
    val nPerformers: Map<String, Int>
)

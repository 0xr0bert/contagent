package sh.greener.contagent.lib.json

import kotlinx.serialization.Serializable

@Serializable
data class AgentSpec(
    val uuid: String,
    val actions: List<String>,
    val activations: List<Map<String, Double>>,
    val deltas: Map<String, Double>,
    val friends: Map<String, Double>,
    val performanceRelationships: Map<String, Map<String, Double>>
)

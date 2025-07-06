package sh.greener.contagent.lib.json

import kotlinx.serialization.Serializable

@Serializable
data class BeliefSpec(
    val uuid: String,
    val name: String,
    val relationships: Map<String, Double>,
    val perceptions: Map<String, Double>
)

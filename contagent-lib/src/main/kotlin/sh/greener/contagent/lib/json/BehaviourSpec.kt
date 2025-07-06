package sh.greener.contagent.lib.json

import kotlinx.serialization.Serializable

@Serializable
data class BehaviourSpec(val name: String, val uuid: String)

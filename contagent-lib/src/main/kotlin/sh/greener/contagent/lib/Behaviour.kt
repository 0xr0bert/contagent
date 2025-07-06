package sh.greener.contagent.lib

import sh.greener.contagent.lib.json.BehaviourSpec
import java.util.UUID

data class Behaviour(override val name: String, override val uuid: UUID) : Named, UUIDd {
    constructor(spec: BehaviourSpec) : this(spec.name, UUID.fromString(spec.uuid))
}

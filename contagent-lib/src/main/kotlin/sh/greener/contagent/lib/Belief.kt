package sh.greener.contagent.lib

import sh.greener.contagent.lib.json.BeliefSpec
import java.util.UUID

data class Belief(
    override val name: String, override val uuid: UUID, val relationships: MutableMap<Belief, Double>,
    val perceptions: Map<Behaviour, Double>
) : Named, UUIDd {
    constructor(spec: BeliefSpec, behavioursByUuid: Map<String, Behaviour>) : this(
        spec.name,
        UUID.fromString(spec.uuid),
        HashMap(),
        spec.perceptions.mapKeys { behavioursByUuid[it.key]!! }
    )

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as Belief

        return uuid == other.uuid
    }

    override fun hashCode(): Int {
        return uuid.hashCode()
    }

    override fun toString(): String {
        return "Belief(uuid=$uuid, name='$name')"
    }


}

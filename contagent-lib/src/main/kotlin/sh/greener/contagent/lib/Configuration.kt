package sh.greener.contagent.lib

import io.github.oshai.kotlinlogging.KLogger
import java.io.OutputStream
import kotlin.random.Random

data class Configuration(
    val behaviours: List<Behaviour>,
    val beliefs: List<Belief>,
    val agents: List<Agent>,
    val startTime: Int,
    val endTime: Int,
    val outputStream: OutputStream,
    val fullOutput: Boolean,
    val rng: Random,
    val logger: KLogger
)

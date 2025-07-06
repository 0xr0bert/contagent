package sh.greener.contagent.lib

import kotlin.math.pow
import kotlin.math.sqrt

fun calculateMeanActivation(c: Configuration, t: Int): Map<Belief, Double> {
    val m = HashMap<Belief, Double>()
    c.agents.forEach {
        agent ->
            agent.activations[t].forEach {
                if (!m.contains(it.key)) {
                    m.put(it.key, 0.0)
                }
                m.put(it.key, m[it.key]!! + it.value)
            }
    }

    return m.mapValues { it.value / c.agents.size }
}

fun calculateSdActivation(c: Configuration, t: Int, means: Map<Belief, Double>): Map<Belief, Double> {
    val m = HashMap<Belief, Double>()
    c.agents.forEach { agent ->
        agent.activations[t].forEach {
            if (!m.contains(it.key)) {
                m.put(it.key, 0.0)
            }
            m.put(it.key, m[it.key]!! + (it.value - means[it.key]!!).pow(2))
        }
    }

    return m.mapValues { sqrt(it.value / (c.agents.size - 1)) }
}

fun calculateMedianActivation(c: Configuration, t: Int): Map<Belief, Double> {
    val allActs = HashMap<Belief, MutableList<Double>>()
    c.beliefs.forEach { allActs.put(it, ArrayList()) }

    c.agents.forEach { agent ->
        agent.activations[t].forEach {
            allActs[it.key]!!.add(it.value)
        }
    }

    allActs.mapValues {
        it.value.sorted()
    }

    val ix = c.agents.size / 2 - 1
    val isEven = c.agents.size % 2 == 0

    return allActs.map {
        it.key to if (isEven) {
            (it.value[ix] + it.value[ix + 1] ) / 2.0
        } else {
            it.value[ix]
        }
    }.toMap()
}

fun calculateNonzeroActivation(c: Configuration, t: Int) : Map<Belief, Int> {
    val m = HashMap<Belief, Int>()

    c.agents.forEach { agent ->
        agent.activations[t].forEach {
            if (it.value != 0.0) {
                if (!m.contains(it.key)) {
                    m.put(it.key, 1)
                } else {
                    m.put(it.key, m[it.key]!! + 1)
                }
            }
        }
    }

    return m
}

fun calculateNPerformers(c: Configuration, t: Int): Map<Behaviour, Int> {
    return c.agents.map { it.actions[t] }.groupingBy { it }.eachCount()
}

fun calculateSummaryStats(c: Configuration, t: Int): SummaryStats {
    val means = calculateMeanActivation(c, t)
    return SummaryStats(
        means,
        calculateSdActivation(c, t, means),
        calculateMedianActivation(c, t),
        calculateNonzeroActivation(c, t),
        calculateNPerformers(c, t)
    )
}
package pl.itachi

import java.io.File

val INITIAL_PATTERN = Regex("initial state: (.*)")
val PRODUCTION_PATTERN = Regex("(.*) => (.)")

fun main(args: Array<String>) {
    var lastPlantState = ""
    val rules: MutableMap<Regex, Char> = mutableMapOf()

    val lines = File("input.txt").readLines()

    INITIAL_PATTERN.find(lines[0])?.let {
        if (it.groups.isNotEmpty()) {
            lastPlantState = it.groups[1]?.value ?: ""
        }
    }

    for (i in 2 until lines.size) {
        PRODUCTION_PATTERN.find(lines[i])?.let {
            if (it.groups.size == 3) {
                val pattern = "(?=${(it.groups[1]?.value ?: "").replace(".", "\\.")})"
                rules[Regex(pattern)] = it.groups[2]?.value?.get(0) ?: '.'
            }
        }
    }

    var offset: Long = -5
    
    // for (i in 1..20) {
    for (i in 1..50000000000) {
        val base = lastPlantState.trim('.')
        val oldState = ".....$base....."
        val newState = Array(oldState.length) { '.' }

        for ((key, value) in rules) {
            key.findAll(oldState).forEach {
                newState[it.range.start + 2] = value
            }
        }

        val result = newState.joinToString(separator = "")
        offset += result.indexOf('#') - 5

        lastPlantState = result

        if (result.trim('.') == base) {
            val repeatingOffset = result.indexOf('#') - 5
            offset += repeatingOffset.toLong() * (50000000000 - i)

            break
        }
    }

    var sum: Long = 0

    ".....${lastPlantState.trim('.')}.....".forEach {
        if (it == '#') {
            sum += offset
        }

        offset++
    }

    println("Result: $sum")
}

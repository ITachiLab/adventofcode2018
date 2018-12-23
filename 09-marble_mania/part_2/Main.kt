package pl.itachi

import java.io.File

class CircularList<T>(c: MutableCollection<out T>?) : ArrayList<T>(c) {
    fun calcIndex(index: Int): Int = when {
        index >= 0 -> index % size
        else -> (size + index) % size
    }

    fun addCircular(index: Int, element: T): Int {
        val putIndex = calcIndex(index)

        if (putIndex == 0) {
            super.add(element)

            return size - 1
        }

        super.add(putIndex, element)

        return putIndex
    }

    override fun get(index: Int): T {
        return super.get(calcIndex(index))
    }

    override fun removeAt(index: Int): T {
        return super.removeAt(calcIndex(index))
    }

    override fun set(index: Int, element: T): T {
        return super.set(calcIndex(index), element)
    }
}

fun main(args: Array<String>) {
    val readLines = File("input.txt").readLines()
    val regex = Regex("^(\\d+).+?(\\d+).*\$")

    var players = 0
    var lastValue = 0

    regex.matchEntire(readLines.first())?.let {
        players = it.groups[1]?.value?.toInt() ?: 0
        lastValue = it.groups[2]?.value?.toInt() ?: 0
    }

    val marbles: CircularList<Int> = CircularList(mutableListOf(0))
    val score = LongArray(players) { 0 }

    var currentMarbleIndex = 0
    var marbleValue = 1
    var currentPlayer = 1

    while (marbleValue <= lastValue) {
        if (marbleValue % 23 == 0) {
            val removeIndex = marbles.calcIndex(currentMarbleIndex - 7)
            score[currentPlayer - 1] = score[currentPlayer - 1] + marbleValue + marbles.removeAt(removeIndex)

            currentMarbleIndex = removeIndex
        } else {
            currentMarbleIndex = marbles.addCircular(currentMarbleIndex + 2, marbleValue)
        }

        marbleValue++

        if (++currentPlayer > players) {
            currentPlayer = 1
        }
    }

    println("Highest score: ${score.max()}")
}
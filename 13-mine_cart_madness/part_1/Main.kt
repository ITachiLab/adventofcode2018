package pl.itachi

import java.io.File

val map: MutableList<CharArray> = mutableListOf()
val carts: MutableList<Cart> = mutableListOf()

enum class CartTurn(val shift: Int) {
    LEFT(-1), STRAIGHT(0), RIGHT(1)
}

class Cart(var x: Int, var y: Int, initialDirection: Char) : Comparable<Cart> {
    override fun compareTo(other: Cart): Int {
        return when {
            y < other.y -> 1
            y == other.y -> when {
                x < other.x -> 1
                x > other.x -> -1
                else -> 0
            }
            else -> -1
        }
    }

    var direction: Int = when(initialDirection) {
        '^' -> 0
        '>' -> 1
        'v' -> 2
        else -> 3
    }

    var nextTurn = CartTurn.LEFT

    private fun alterDirection() {
        direction += nextTurn.shift

        direction = when {
            direction < 0 -> 3
            direction > 3 -> 0
            else -> direction
        }

        nextTurn = when (nextTurn) {
            CartTurn.LEFT -> CartTurn.STRAIGHT
            CartTurn.STRAIGHT -> CartTurn.RIGHT
            CartTurn.RIGHT -> CartTurn.LEFT
        }
    }

    fun move(): Cart? {
        when (direction) {
            0 -> y--
            1 -> x++
            2 -> y++
            else -> x--
        }

        when (map[y][x]) {
            '+' -> alterDirection()
            '/' -> direction = when(direction) {
                0 -> 1
                1 -> 0
                2 -> 3
                else -> 2
            }
            '\\' -> direction = when(direction) {
                0 -> 3
                1 -> 2
                2 -> 1
                else -> 0
            }
        }

        carts.forEach {
            if (it != this && x == it.x && y == it.y) {
                return this
            }
        }

        return null
    }
}

fun main(args: Array<String>) {
    var rowNumber = 0

    File("input.txt").forEachLine {
        val array = it.toCharArray()

        for (i in 0 until array.size) {
            if (array[i] in arrayOf('>', '<')) {
                carts.add(Cart(i, rowNumber, array[i]))
                array[i] = '-'
            } else if (array[i] in arrayOf('^', 'v')) {
                carts.add(Cart(i, rowNumber, array[i]))
                array[i] = '|'
            }
        }

        map.add(array)
        rowNumber++
    }

    var crashCart: Cart? = null

    while (crashCart == null) {
        carts.sorted().forEach { cart ->
            cart.move()?.let {
                crashCart = it
                return@forEach
            }
        }
    }

    println("Crash at: ${crashCart?.x},${crashCart?.y}")
}

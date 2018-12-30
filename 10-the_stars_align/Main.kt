package pl.itachi

import java.io.File
import kotlin.math.min

data class Point(var x: Int, var y: Int, val vX: Int, val vY: Int) {
    fun move() {
        x += vX
        y += vY
    }
}

fun main(args: Array<String>) {
    val points: MutableList<Point> = mutableListOf()
    val pattern = Regex("position=<(.+),\\s+(.+)>\\svelocity=<(.+),\\s(.+)>")

    File("input.txt").forEachLine { line ->
        val groups = pattern.find(line)?.groups

        if (groups != null) {
            val x: Int = groups[1]?.value?.trim()?.toInt() ?: throw Error("Invalid value X")
            val y: Int = groups[2]?.value?.trim()?.toInt() ?: throw Error("Invalid value Y")
            val vX: Int = groups[3]?.value?.trim()?.toInt() ?: throw Error("Invalid value VX")
            val vY: Int = groups[4]?.value?.trim()?.toInt() ?: throw Error("Invalid value VY")

            points.add(Point(x, y, vX, vY))
        }
    }

    for (i in 0..10000000) {
        val count = points.groupBy { it.x }.values.filter { it.size >= 8 }.count()

        if (count > 15) {
            val minimum = min(points.minBy { it.x }?.x ?: 0, points.minBy { it.y }?.y ?: 0)
            val maximum = points.maxBy { it.x }?.x ?: Int.MAX_VALUE

            for (y in 0 until maximum) {
                for (x in 0 until maximum) {
                    val found = points.firstOrNull() { it.x - minimum == x && it.y - minimum == y }

                    if (found == null) {
                        print(".")
                    } else {
                        print("#")
                    }
                }

                print("\n")
            }

            println("Seconds: $i")

            break
        }

        points.forEach { it.move() }
    }
}
package pl.itachi

const val GRID_WIDTH = 300
const val GRID_HEIGHT = 300
const val GRID_ID = 3613

var grid: Array<Array<PowerRack>> = Array(GRID_HEIGHT) { y -> Array(GRID_WIDTH) { x -> PowerRack(x, y) } }

class PowerRack(val x: Int, val y: Int) {
    val powerSum: MutableMap<Int, Int> = mutableMapOf()
    var primaryValue = 0

    init {
        primaryValue = calcXYPower(x, y)
        powerSum[1] = primaryValue
    }

    fun value(size: Int = 1): Int? {
        if ((x + (size - 1) >= GRID_WIDTH) || (y + (size - 1) >= GRID_HEIGHT)) {
            return null
        }

        if (size == 1) {
            return primaryValue
        }

        if (powerSum.containsKey(size)) {
            return powerSum.getValue(size)
        }

        var value = this.value(size - 1)

        if (value != null) {
            for (gridY in 0 until size) {
                value += grid[y + gridY][x + (size - 1)].value() ?: 0
            }

            for (gridX in 0 until size - 1) {
                value += grid[y + (size - 1)][x + gridX].value() ?: 0
            }

            return value.also { powerSum[size] = it }
        }

        return null
    }
}

fun calcXYPower(x: Int, y: Int): Int {
    val rackId = (x + 1) + 10
    var powerLvl = rackId * (y + 1)

    powerLvl += GRID_ID
    powerLvl *= rackId
    powerLvl = (powerLvl / 100) % 10
    powerLvl -= 5

    return powerLvl
}

fun main(args: Array<String>) {
    val max = grid.flatten().filter { it.value(3) != null }.maxBy { it.value(3)!! }

    if (max != null) {
        println("ID: ${max.x + 1},${max.y + 1}")
    }
}
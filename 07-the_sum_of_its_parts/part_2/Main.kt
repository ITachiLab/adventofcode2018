package pl.itachi

import java.io.File

const val workers = 5

val toDo: MutableSet<Node> = mutableSetOf()
val nodes: MutableMap<Char, Node> = mutableMapOf()
var seconds = 0

class Node(val name: Char) : Comparable<Node> {
    val children: MutableList<Node> = mutableListOf()

    val parents: MutableList<Node> = mutableListOf()
    var taken: Boolean = false

    var remainingTime = (name - 'A') + 1 + 60

    fun addChildren(childNode: Node) {
        childNode.parents.add(this)
        children.add(childNode)
    }

    fun canBeDone(): Boolean {
        return parents.all { it.isDone() }
    }

    fun isDone() = remainingTime == 0

    fun doWork() = remainingTime--
    override fun compareTo(other: Node): Int {
        return name.compareTo(other.name)
    }

}

fun traverse() {
    val currentWork: MutableList<Node> = mutableListOf()

    do {
        toDo.addAll(nodes.values.filter { !it.isDone() && it.canBeDone() && !it.taken })
        val n = toDo.sorted().take(workers)

        n.forEach {
            if (!currentWork.contains(it) && currentWork.size < workers) {
                currentWork.add(it)
            }
        }

        println (currentWork.joinToString { it.name.toString() })
        seconds++

        currentWork.forEach {
            it.taken = true
            it.doWork()

            if (it.isDone()) {
                toDo.remove(it)
            }
        }

        currentWork.removeAll { it.isDone() }
    } while (nodes.values.any { !it.isDone() } )
}

fun main(args: Array<String>) {
    File("input.txt").forEachLine { line ->
        val pattern = Regex("Step ([A-Z]) must be finished before step ([A-Z]) can begin\\.")
        val find = pattern.matchEntire(line)

        if (find != null) {
            val requirement: Char? = find.groups[1]?.value?.get(0)
            val product: Char? = find.groups[2]?.value?.get(0)

            if (requirement != null && product != null) {
                val reqNode = nodes.getOrElse(requirement) { Node(requirement) }.also { nodes[requirement] = it }
                val prodNode = nodes.getOrElse(product) { Node(product) }.also { nodes[product] = it }

                reqNode.addChildren(prodNode)
            }
        }
    }

    val node = Node('#').also { it.remainingTime = 0 }
    nodes.values.filter { it.parents.isEmpty() }.forEach { node.addChildren(it) }

    traverse()

    println(seconds)
}
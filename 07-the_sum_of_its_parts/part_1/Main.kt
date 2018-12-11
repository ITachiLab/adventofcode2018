package pl.itachi

import java.io.File

class Node(val name: Char) : Comparable<Node> {
    private val children: MutableList<Node> = mutableListOf()
    private val parents: MutableList<Node> = mutableListOf()

    var visited: Boolean = false

    fun addChildren(childNode: Node) {
        childNode.parents.add(this)
        children.add(childNode)
    }

    fun canBeDone(): Boolean {
        return parents.all { it.visited }
    }

    override fun compareTo(other: Node): Int {
        return name.compareTo(other.name)
    }
}

fun main(args: Array<String>) {
    val nodes: MutableMap<Char, Node> = mutableMapOf()

    File("input.txt").forEachLine { line ->
        val pattern = Regex("Step ([A-Z]) must be finished before step ([A-Z]) can begin\\.")
        val find = pattern.matchEntire(line)

        if (find != null) {
            val requirement: Char? = find.groups[1]?.value?.get(0)
            val product: Char? = find.groups[2]?.value?.get(0)

            if (requirement != null && product != null) {
                nodes.getOrElse()
                val reqNode = Node(requirement)
                val prodNode = Node(product)

                reqNode.addChildren(prodNode)
            }
        }
    }
}

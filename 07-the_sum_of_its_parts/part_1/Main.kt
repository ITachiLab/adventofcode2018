package pl.itachi

import java.io.File

class Node(val name: Char) : Comparable<Node> {
    val children: MutableList<Node> = mutableListOf()
    val parents: MutableList<Node> = mutableListOf()

    fun addChildren(childNode: Node) {
        childNode.parents.add(this)
        children.add(childNode)
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
                val reqNode = nodes.getOrElse(requirement) {Node(requirement)}.also { nodes[requirement] = it }
                val prodNode = nodes.getOrElse(product) {Node(product)}.also { nodes[product] = it }

                reqNode.addChildren(prodNode)
            }
        }
    }

    val nodeQueue: MutableList<Node> = mutableListOf()

    nodeQueue.addAll(nodes.values.filter { it.parents.isEmpty() }.sorted())

    while (nodeQueue.isNotEmpty()) {
        val node = nodeQueue.first().also { nodeQueue.remove(it) }
        print(node.name)

        node.children
            .sorted()
            .forEach {
                it.parents.remove(node)

                if (it.parents.isEmpty()) {
                    nodeQueue.add(it).also { nodeQueue.sort() }
                }
            }
    }
}
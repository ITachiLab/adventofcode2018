package pl.itachi

import java.io.BufferedReader
import java.io.File

class Node(val childrenCount: Int, val metadataCount: Int) {
    val children: MutableList<Node> = mutableListOf()
    val metadata: MutableList<Int> = mutableListOf()
}

fun readNextInt(reader: BufferedReader): Int {
    val readChars: MutableList<Char> = mutableListOf()

    do {
        val read = reader.read()

        if (read in 0x30..0x39) {
            readChars.add(read.toChar())
        }
    } while (read in 0x30..0x39)

    return readChars.joinToString(separator = "").toInt()
}

fun createNode(reader: BufferedReader) : Node {
    val childrenCount = readNextInt(reader)
    val metadataCount = readNextInt(reader)

    val node = Node(childrenCount, metadataCount)

    for (i in 0 until childrenCount) {
        node.children.add(createNode(reader))
    }

    for (i in 0 until metadataCount) {
        node.metadata.add(readNextInt(reader))
    }

    return node
}

fun sumEntries(root: Node, sum: Int): Int {
    var s = sum

    root.children.forEach { s = sumEntries(it, s) }

    return root.metadata.sum() + s
}

fun main(args: Array<String>) {
    val reader = File("input.txt").bufferedReader()

    val root = createNode(reader)
    print(sumEntries(root, 0))

    reader.close()
}

def compare(l1, l2):
    diffs = 0
    index = 0

    for i in range(0, len(l1)):
        if l1[i] != l2[i]:
            index = i
            diffs += 1

        if diffs > 1:
            return -1, -1

    return diffs, index

def main():
    with open('input.txt') as f:
        lines = []
        start = 0

        for line in f:
            if len(line) > 1:
                lines.append(line[:-1])

        for i in range(start, len(lines)):
            for j in range(i + 1, len(lines)):
                l1 = list(lines[i])
                l2 = list(lines[j])

                diffs, index = compare(l1, l2)

                if diffs == 1:
                    del l1[index]
                    del l2[index]

                    print ''.join(l1)
                    print ''.join(l2)

if __name__ == '__main__':
    main()

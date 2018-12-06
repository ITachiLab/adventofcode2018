from collections import defaultdict

def main():
    twos = 0
    threes = 0

    with open('input2.txt') as f:
        for line in f:
            count = defaultdict(int)

            for c in line:
                count[c] += 1

            if 2 in count.values():
                twos += 1
            if 3 in count.values():
                threes += 1

    print(twos*threes)


if __name__ == '__main__':
    main()


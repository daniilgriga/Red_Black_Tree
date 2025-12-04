import os
import random

def solve_range_query (tree_set, low, high):
    if low > high:
        return 0
    count = 0
    for val in tree_set:
        if low <= val <= high:
            count += 1
    return count

def write_test (test_id, commands, folder="."):
    filename = f"{test_id:03d}"
    path_dat = os.path.join (folder, f"{filename}.dat")
    path_ans = os.path.join (folder, f"{filename}.ans")

    tree_set = set()
    results = []

    with open (path_dat, "w") as f:
        tokens = []
        for cmd in commands:
            if cmd[0] == 'k':
                tokens.append (f"k {cmd[1]}")
                tree_set.add (cmd[1])
            elif cmd[0] == 'q':
                tokens.append (f"q {cmd[1]} {cmd[2]}")
                result = solve_range_query (tree_set, cmd[1], cmd[2])
                results.append (result)
        f.write (" ".join(tokens))

    with open (path_ans, "w") as f:
        if results:
            f.write (" ".join(map(str, results)))
        else:
            f.write ("")

def main():
    test_cases = []

    test_cases.append ([
        ('k', 10),
        ('q', 5, 15)
    ])

    test_cases.append ([
        ('k', 5),
        ('k', 10),
        ('k', 15),
        ('q', 0, 20)
    ])

    test_cases.append ([
        ('k', 100),
        ('k', 200),
        ('q', 1, 50)
    ])

    test_cases.append ([
        ('k', 10),
        ('k', 20),
        ('k', 30),
        ('q', 5, 15),
        ('q', 15, 25),
        ('q', 25, 35)
    ])

    test_cases.append ([
        ('k', 50),
        ('k', 50),
        ('k', 50),
        ('q', 40, 60)
    ])

    test_cases.append ([
        ('q', 1, 100)
    ])

    test_cases.append ([
        ('k', 25),
        ('k', 75),
        ('q', 100, 50)
    ])

    test_cases.append ([
        ('k', 10),
        ('k', 20),
        ('k', 30),
        ('q', 10, 30),
        ('q', 11, 29),
        ('q', 10, 10)
    ])

    test_cases.append ([
        ('k', i) for i in range(1, 21)
        ] + [
        ('q', 5, 15),
        ('q', 1, 20)
    ])

    test_cases.append ([
        ('k', i) for i in range(20, 0, -1)
        ] + [
        ('q', 5, 15),
        ('q', 1, 20)
    ])

    # 10-15: 100-500 operations
    random.seed (42)
    for _ in range (5):
        n_ops = random.randint (100, 500)
        cmds = []
        for _ in range (n_ops):
            if random.random() < 0.7:
                cmds.append (('k', random.randint (1, 1000)))
            else:
                low = random.randint (1, 900)
                high = low + random.randint (0, 200)
                cmds.append (('q', low, high))
        test_cases.append (cmds)

    # 16-18: 1000-5000 operations
    for _ in range (3):
        n_ops = random.randint (1000, 5000)
        cmds = []
        for _ in range (n_ops):
            if random.random() < 0.75:
                cmds.append (('k', random.randint(1, 10000)))
            else:
                low = random.randint (1, 9000)
                high = low + random.randint (0, 1000)
                cmds.append (('q', low, high))
        test_cases.append (cmds)

    # 19: Stress with many duplicates (1K operations)
    cmds = []
    for _ in range (1000):
        cmds.append (('k', random.randint(1, 50)))
    for _ in range (100):
        low = random.randint (1, 40)
        high = low + random.randint (0, 20)
        cmds.append (('q', low, high))
    test_cases.append (cmds)

    # 20: Huge stress (10K operations)
    cmds = []
    for _ in range (10000):
        if random.random() < 0.8:
            cmds.append (('k', random.randint(1, 100000)))
        else:
            low = random.randint (1, 90000)
            high = low + random.randint (0, 10000)
            cmds.append (('q', low, high))
    test_cases.append (cmds)

    #  21: 50K operations
    cmds = []
    for _ in range (50000):
        if random.random() < 0.8:
            cmds.append (('k', random.randint(1, 500000)))
        else:
            low = random.randint (1, 450000)
            high = low + random.randint (0, 50000)
            cmds.append (('q', low, high))
    test_cases.append (cmds)

    # 22: 100K operations
    cmds = []
    for _ in range(100000):
        if random.random() < 0.85:
            cmds.append (('k', random.randint (1, 1000000)))
        else:
            low = random.randint (1, 900000)
            high = low + random.randint (0, 100000)
            cmds.append (('q', low, high))
    test_cases.append (cmds)

    # 23: 200K operations
    cmds = []
    for _ in range (200000):
        if random.random() < 0.7:
            cmds.append (('k', random.randint (1, 2000000)))
        else:
            low = random.randint (1, 1800000)
            high = low + random.randint (0, 200000)
            cmds.append (('q', low, high))
    test_cases.append (cmds)

    for i, test in enumerate (test_cases, 1):
        write_test (i, test, folder=".")
        print (f"Generated test {i:03d}: {len(test)} operations")

    print (f"Total: {len(test_cases)} tests generated (001.dat/ans to {len(test_cases):03d}.dat/ans)")

if __name__ == "__main__":
    main()

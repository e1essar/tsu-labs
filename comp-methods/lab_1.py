grammar = {
    "A": ["!B!"],
    "B": ["T", "T+B"],
    "T": ["M", "M*T"],
    "M": ["a", "b", "(B)"],
}

rules_idx = {
    ("A", 0): 1,
    ("B", 0): 2,
    ("B", 1): 3,
    ("T", 0): 4,
    ("T", 1): 5,
    ("M", 0): 6,
    ("M", 1): 7,
    ("M", 2): 8
}

T = ['!', '+', '*', '(', ')', 'a', 'b']

def nondet_parse(string):
    L1 = []
    L2  = "A" # starting symbol
    state = 'q' # q - normal, b - backup, t - end
    i = 0

    while state != 't':
        if state == 'q':
            if L2[0] not in T:
                L1.append([L2[0], 0])
                L2 = L2.replace(L2[0], grammar[L2[0]][0], 1)
            else:
                if L2[0] != string[i]:
                    state = 'b'
                else:
                    L1.append(L2[0])
                    L2 = L2[1:]
                    i += 1
                    if i == len(string):
                        state = 't' if L2 == "" else 'b'
                    elif L2 == "":
                        state = 'b'
        elif state == 'b':
            if L1[-1] in T:
                i -= 1
                L2 = L1[-1] + L2
                L1.pop()
            else:
                if len(grammar[L1[-1][0]]) - 1 > L1[-1][1]:
                    L1[-1][1] += 1
                    L2 = L2.replace(L2[0], grammar[L1[-1][0]][L1[-1][1]], 1)
                    state = 'q'
                elif L1[-1][0] == "A" and i == 0:
                    print("Error")
                    return
                else:
                    L2 = L2.replace(grammar[L1[-1][0]][L1[-1][1]], L1[-1][0], 1)
                    L1.pop()
    return L1

def rule_printer(rules):
    result = []
    if rules is not None:
        for rule in rules:
            if tuple(rule) in rules_idx:
                result.append(rules_idx[tuple(rule)])
    else:
        result = ""
    print(result)


str = input()
result = nondet_parse(str)
rule_printer(result)

# str = input()
# result = nondet_parse(str)
# rule_printer(result)

# str = "!a+b!"
# str = "!a*b!"
# str = "!(a+b)*(b+a)!"
# str = "!b*a+a*b!"
# str = "!(a+b)*a+b*a!"
# str = "!(a+b*a)*(b*b+a*(a+b+a))!"
# str = "!a+*b!"
# str = "a+b*a+b"
# str = "a!b"
# str = "!a(b+a()!"

# NFA Epsilon Transition Elimination

A simple C program that removes epsilon (ε) transitions from a Non-Deterministic Finite Automaton (NFA).

## How to Compile

```bash
gcc nfa_epsilon.c -o nfa_epsilon
```

## How to Run

```bash
./nfa_epsilon
```

## Input Format

The program will prompt you for:
1. Number of states
2. Number of symbols (excluding epsilon)
3. The symbols themselves (e.g., a b c)
4. Start state
5. Number of final states
6. Final states
7. Regular transitions (format: `state symbol destination`, enter -1 to end)
8. Epsilon transitions (format: `state destination`, enter -1 to end)

## Example Input

```
Enter number of states: 3
Enter number of symbols (excluding epsilon): 1
Enter symbols (e.g., a b c): a
Enter start state: 0
Enter number of final states: 1
Enter final states: 2

Enter transitions (state symbol destination, -1 to end):
0 a 1
-1

Enter epsilon transitions (state destination, -1 to end):
0 1
1 2
-1
```

## Output

The program will:
1. Display epsilon closures for each state
2. Show the new NFA without epsilon transitions
3. List all new transitions and final states

## Files

- `nfa_epsilon.c` - Main program
- `EXPLANATION.md` - Detailed code explanation
- `README.md` - This file


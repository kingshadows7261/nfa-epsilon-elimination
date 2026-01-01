//MAKHLOUFI Nadji Dhia Eddine G02 AI
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 100
#define MAX_SYMBOLS 10

// Structure to represent the NFA
typedef struct {
    int num_states;
    int num_symbols;
    int start_state;
    int final_states[MAX_STATES];
    int num_final;
    char symbols[MAX_SYMBOLS];
    
    // Transition table: transitions[state][symbol] = array of destination states
    int transitions[MAX_STATES][MAX_SYMBOLS][MAX_STATES];
    int transition_count[MAX_STATES][MAX_SYMBOLS];
    
    // Epsilon transitions: epsilon_transitions[state] = array of destination states
    int epsilon_transitions[MAX_STATES][MAX_STATES];
    int epsilon_count[MAX_STATES];
} NFA;

// Global NFA variable
NFA nfa;

// Task 1: Read the NFA from input
void read_nfa() {
    printf("Enter number of states: ");
    scanf("%d", &nfa.num_states);
    
    printf("Enter number of symbols (excluding epsilon): ");
    scanf("%d", &nfa.num_symbols);
    
    printf("Enter symbols (e.g., a b c): ");
    for (int i = 0; i < nfa.num_symbols; i++) {
        scanf(" %c", &nfa.symbols[i]);
    }
    
    printf("Enter start state: ");
    scanf("%d", &nfa.start_state);
    
    printf("Enter number of final states: ");
    scanf("%d", &nfa.num_final);
    
    printf("Enter final states: ");
    for (int i = 0; i < nfa.num_final; i++) {
        scanf("%d", &nfa.final_states[i]);
    }
    
    // Initialize transition counts
    for (int i = 0; i < MAX_STATES; i++) {
        for (int j = 0; j < MAX_SYMBOLS; j++) {
            nfa.transition_count[i][j] = 0;
        }
        nfa.epsilon_count[i] = 0;
    }
    
    // Read regular transitions
    printf("\nEnter transitions (state symbol destination, -1 to end):\n");
    int from, to;
    char symbol;
    while (1) {
        scanf("%d", &from);
        if (from == -1) break;
        scanf(" %c", &symbol);
        scanf("%d", &to);
        
        // Find symbol index
        int sym_idx = -1;
        for (int i = 0; i < nfa.num_symbols; i++) {
            if (nfa.symbols[i] == symbol) {
                sym_idx = i;
                break;
            }
        }
        
        if (sym_idx != -1) {
            int count = nfa.transition_count[from][sym_idx];
            nfa.transitions[from][sym_idx][count] = to;
            nfa.transition_count[from][sym_idx]++;
        }
    }
    
    // Read epsilon transitions
    printf("\nEnter epsilon transitions (state destination, -1 to end):\n");
    while (1) {
        scanf("%d", &from);
        if (from == -1) break;
        scanf("%d", &to);
        
        int count = nfa.epsilon_count[from];
        nfa.epsilon_transitions[from][count] = to;
        nfa.epsilon_count[from]++;
    }
}

// Task 2: Calculate epsilon closure of a state
void calculate_epsilon_closure(int state, int visited[], int closure[], int *closure_size) {
    // Mark current state as visited and add to closure
    if (visited[state]) return;
    visited[state] = 1;
    closure[*closure_size] = state;
    (*closure_size)++;
    
    // Recursively visit all states reachable via epsilon
    for (int i = 0; i < nfa.epsilon_count[state]; i++) {
        int next_state = nfa.epsilon_transitions[state][i];
        calculate_epsilon_closure(next_state, visited, closure, closure_size);
    }
}

// Task 3: Compute new transitions without epsilon
void compute_new_transitions(int new_transitions[][MAX_SYMBOLS][MAX_STATES], 
                             int new_transition_count[][MAX_SYMBOLS],
                             int epsilon_closures[][MAX_STATES],
                             int closure_sizes[]) {
    
    // For each state
    for (int state = 0; state < nfa.num_states; state++) {
        // For each symbol
        for (int sym = 0; sym < nfa.num_symbols; sym++) {
            int destinations[MAX_STATES];
            int dest_count = 0;
            int dest_visited[MAX_STATES] = {0};
            
            // For each state in epsilon closure of current state
            for (int i = 0; i < closure_sizes[state]; i++) {
                int closure_state = epsilon_closures[state][i];
                
                // Follow transitions on symbol from closure state
                for (int j = 0; j < nfa.transition_count[closure_state][sym]; j++) {
                    int dest = nfa.transitions[closure_state][sym][j];
                    
                    // Add all states in epsilon closure of destination
                    for (int k = 0; k < closure_sizes[dest]; k++) {
                        int final_dest = epsilon_closures[dest][k];
                        if (!dest_visited[final_dest]) {
                            destinations[dest_count++] = final_dest;
                            dest_visited[final_dest] = 1;
                        }
                    }
                }
            }
            
            // Store new transitions
            new_transition_count[state][sym] = dest_count;
            for (int i = 0; i < dest_count; i++) {
                new_transitions[state][sym][i] = destinations[i];
            }
        }
    }
}

// Task 4: Determine new final states
void determine_new_final_states(int new_final[], int *new_final_count,
                                int epsilon_closures[][MAX_STATES],
                                int closure_sizes[]) {
    *new_final_count = 0;
    
    // A state is final if its epsilon closure contains any original final state
    for (int state = 0; state < nfa.num_states; state++) {
        for (int i = 0; i < closure_sizes[state]; i++) {
            int closure_state = epsilon_closures[state][i];
            
            // Check if closure state is a final state
            for (int j = 0; j < nfa.num_final; j++) {
                if (closure_state == nfa.final_states[j]) {
                    new_final[*new_final_count] = state;
                    (*new_final_count)++;
                    goto next_state; // Found final, move to next state
                }
            }
        }
        next_state:;
    }
}

// Task 5: Display the new automaton
void display_new_automaton(int new_transitions[][MAX_SYMBOLS][MAX_STATES],
                           int new_transition_count[][MAX_SYMBOLS],
                           int new_final[], int new_final_count) {
    printf("\n=== NFA WITHOUT EPSILON TRANSITIONS ===\n\n");
    
    printf("Number of states: %d\n", nfa.num_states);
    printf("Number of symbols: %d\n", nfa.num_symbols);
    printf("Symbols: ");
    for (int i = 0; i < nfa.num_symbols; i++) {
        printf("%c ", nfa.symbols[i]);
    }
    printf("\n");
    printf("Start state: %d\n", nfa.start_state);
    
    printf("\nTransitions:\n");
    for (int state = 0; state < nfa.num_states; state++) {
        for (int sym = 0; sym < nfa.num_symbols; sym++) {
            if (new_transition_count[state][sym] > 0) {
                for (int i = 0; i < new_transition_count[state][sym]; i++) {
                    printf("δ(%d, %c) = %d\n", state, nfa.symbols[sym], 
                           new_transitions[state][sym][i]);
                }
            }
        }
    }
    
    printf("\nFinal states: ");
    for (int i = 0; i < new_final_count; i++) {
        printf("%d ", new_final[i]);
    }
    printf("\n");
}

// Main function
int main() {
    printf("=== NFA EPSILON TRANSITION ELIMINATION ===\n\n");
    
    // Task 1: Read the NFA
    read_nfa();
    
    // Task 2: Calculate epsilon closure for each state
    int epsilon_closures[MAX_STATES][MAX_STATES];
    int closure_sizes[MAX_STATES];
    
    printf("\n=== EPSILON CLOSURES ===\n");
    for (int state = 0; state < nfa.num_states; state++) {
        int visited[MAX_STATES] = {0};
        closure_sizes[state] = 0;
        calculate_epsilon_closure(state, visited, epsilon_closures[state], 
                                  &closure_sizes[state]);
        
        printf("ε-closure(%d) = {", state);
        for (int i = 0; i < closure_sizes[state]; i++) {
            printf("%d", epsilon_closures[state][i]);
            if (i < closure_sizes[state] - 1) printf(", ");
        }
        printf("}\n");
    }
    
    // Task 3: Compute new transitions
    int new_transitions[MAX_STATES][MAX_SYMBOLS][MAX_STATES];
    int new_transition_count[MAX_STATES][MAX_SYMBOLS];
    compute_new_transitions(new_transitions, new_transition_count,
                           epsilon_closures, closure_sizes);
    
    // Task 4: Determine new final states
    int new_final[MAX_STATES];
    int new_final_count;
    determine_new_final_states(new_final, &new_final_count,
                              epsilon_closures, closure_sizes);
    
    // Task 5: Display the new automaton
    display_new_automaton(new_transitions, new_transition_count,
                         new_final, new_final_count);
    
    return 0;
}


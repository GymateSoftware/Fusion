// Copyright © 2023 Gymate, Inc.
//
// Licensed under the GNU General Public License v3.0

#include "stack.h"
#include "repl.h"
#include <memory.h>

Stack* create_stack(int size) {
    Stack* stack = malloc(sizeof(Stack));
    if (stack == NULL) { return NULL; }

    stack->entries = malloc(size * sizeof(StackEntry));
    if (stack->entries == NULL) {
        free(stack);
        return NULL;
    }

    stack->max_size = size;
    stack->count = 0;

    return stack;
}

void push(Stack* stack, void* value, LunaType type) {
    if (stack->count < stack->max_size) {
        switch (type) {
            case LUNA_TYPE_BOOLEAN:
                stack->entries[stack->count].type = type;
                stack->entries[stack->count].value.boolean_value = *((LunaBoolean*) value);
                stack->count++;
                break;
            case LUNA_TYPE_NUMBER:
                stack->entries[stack->count].type = type;
                stack->entries[stack->count].value.number_value = *((LunaNumber*) value);
                stack->count++;
                break;
            case LUNA_TYPE_STRING:
                stack->entries[stack->count].type = type;
                stack->entries[stack->count].value.string_value = (LunaString) value;
                stack->count++;
                break;
            case LUNA_TYPE_NULL:
            default:
                print_error("Tried to push null reference to stack.\n");
                break;
        }
    } else {
        // Error: Stack Overflow
        print_error("Stack overflow\n");
        exit(2);
    }
}

void* pop(Stack* stack, LunaType* type) {
    if (stack->count > 0) {
        stack->count--;
        switch (stack->entries[stack->count].type) {
            case LUNA_TYPE_BOOLEAN:
                if (type != NULL) { *type = stack->entries[stack->count].type; }
                *type = stack->entries[stack->count].type;
                return &stack->entries[stack->count].value.boolean_value;
            case LUNA_TYPE_NUMBER:
                if (type != NULL) { *type = stack->entries[stack->count].type; }
                return &stack->entries[stack->count].value.number_value;
            case LUNA_TYPE_STRING:
                if (type != NULL) { *type = stack->entries[stack->count].type; }
                return stack->entries[stack->count].value.string_value;
            case LUNA_TYPE_NULL:
            default:
                print_error("Tried to pop null reference from stack\n");
                exit(1);
        }
    } else {
        // Error: Stack underflow
        return NULL;
    }
}

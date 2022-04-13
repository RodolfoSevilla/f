/* parse.c - parsing and parse tree construction */

#include "project04.h"


void parse_table_init(struct parse_table_st *pt) {
    pt->len = 0;
}

// "Allocate" a parse node from the table of parse nodes
struct parse_node_st * parse_node_new(struct parse_table_st *pt) {
    struct parse_node_st *np;

    np = calloc(1, sizeof(struct parse_node_st));
    pt->len += 1;

    return np;
}

void parse_error(char *err) {
    printf("parse_error: %s\n", err);
    exit(-1);
}

// These are names of operators for printing
char *parse_oper_strings[] = {"PLUS", "MINUS", "MULT", "DIV", "OR", "NOT", "AND", "XOR", "LSHIFT", "RSHIFT"};

// Print the dots which represent tree depth in the output
void parse_tree_print_indent(int level) {
    level *= 2;
    for (int i = 0; i < level; i++) {
        printf(".");
    }
}

// Traverse the tree recursively to print out the structs
void parse_tree_print_expr(struct parse_node_st *np, int level) {
    parse_tree_print_indent(level);
    printf("EXPR ");

    // TODO: add support for oper1
    
    if (np->type == EX_INTVAL) {
        printf("INTVAL %d\n", np->intval.value);
    } else if (np->type == EX_OPER1) {
        printf("OPER1 %s\n", parse_oper_strings[np->oper1.oper]);
        parse_tree_print_expr(np->oper1.operand, level+1);
    } 
    else if (np->type == EX_OPER2) {
        printf("OPER2 %s\n", parse_oper_strings[np->oper2.oper]); 
        parse_tree_print_expr(np->oper2.left, level+1);
        parse_tree_print_expr(np->oper2.right, level+1);
    }
}

int evalute(struct parse_node_st *np){
	if (!np){
		return 0;
	}
	if (!np->oper2.left && !np->oper2.right){
	   return np->intval.value;
	   }
	int lval = evalute(np->oper2.left);
	int rval = evalute(np->oper2.right);
	if (parse_oper_strings[np->oper2.oper] == "PLUS") 
        return lval + rval;
 
    if (parse_oper_strings[np->oper2.oper] == "MINUS")
        return lval - rval;
 
    if (parse_oper_strings[np->oper2.oper] == "MULT")
        return lval * rval;
 	if(parse_oper_strings[np->oper2.oper] == "DIV")
    	return lval / rval;
    if(parse_oper_strings[np->oper2.oper] == "OR")
    	return lval | rval;
    if(parse_oper_strings[np->oper2.oper] == "NOT")
    	return ~lval + ~rval;
    if(parse_oper_strings[np->oper2.oper] == "AND")
	    return lval & rval;
	if(parse_oper_strings[np->oper2.oper] == "XOR")
		return lval ^ rval;
	if(parse_oper_strings[np->oper2.oper] == "LSHIFT")
		return lval << rval;
	if(parse_oper_strings[np->oper2.oper] == "RSHIFT")   
		return lval >> rval;    
}
char *reverse_String(char * reverseString){
	char* curr = malloc(1000);
	for(int i = strlen(reverseString); i >= 0; i--){
		strncat(curr, &reverseString[i], 1);
	}
	return curr;
}

void valueString(int val, int base){
	char *result = malloc(1000);
	int remainder;
	while(val != 0){
		remainder = val % base;
		if(remainder == 10 && base == 16){
			val /= base;
			char myChar = 'A';
			strncat(result, &myChar, 1);
		}
		else if (remainder == 11 && base == 16){
			val /= base;
			char myChar = 'B';
			strncat(result, &myChar, 1);
		}
		else if (remainder == 12 && base == 16){
					val /= base;
					char myChar = 'C';
					strncat(result, &myChar, 1);
		}
		else if (remainder == 13 && base == 16){
					val /= base;
					char myChar = 'D';
					strncat(result, &myChar, 1);
		}
		else if (remainder == 14 && base == 16){
					val /= base;
					char myChar = 'E';
					strncat(result, &myChar, 1);
		}
		else if (remainder == 15 && base == 16){
					val /= base;
					char myChar = 'F';
					strncat(result, &myChar, 1);
		}
		else{
			val /= base;
			char myChar = remainder += '0';
			strncat(result, &myChar, 1);
		}
		}
		if(base == 16){
			printf("0x");
			if(strlen(result) < 8){
				for(size_t i = 8; i > strlen(result); i--){
					printf("0");
				}
			}
			printf("%s\n", reverse_String(result));
		}
		if (base == 2){
			printf("0b");
			if(strlen(result) < 16){
				for(size_t i = 32; i > strlen(result); i--){
					printf("0");
				}
		}
		printf("%s\n", reverse_String(result));
	}
}
void parse_tree_print(struct parse_node_st *np) {
    parse_tree_print_expr(np, 0);
}

// Parse the "program" part of the EBNF
// A program is composed of an expression followed by EOT
struct parse_node_st * parse_program(struct parse_table_st *pt, 
                                        struct scan_table_st *st) {
    struct parse_node_st *np1;

    np1 = parse_expression(pt, st);

    if (!scan_table_accept(st, TK_EOT)) {
        parse_error("Expecting EOT");        
    }

    return np1;                                       
}

// Build the tree for expressions
// Expressions are defined in the EBNF as an operator followed
// by zero or more operator operand pairs
struct parse_node_st * parse_expression(struct parse_table_st *pt, 
                                        struct scan_table_st *st) {
    struct scan_token_st *tp;
    struct parse_node_st *np1, *np2;

    np1 = parse_operand(pt, st);

    /*
        TODO
        add cases for other OPER2 operators
    */
    while (true) {    
        tp = scan_table_get(st, 0);
      
        if(tp->id == TK_MULT ){
          scan_table_accept(st, TK_ANY);
          np2 = parse_node_new(pt);
          np2->type = EX_OPER2;
          np2->oper2.oper = OP_MULT;
          np2->oper2.left = np1;
          np2->oper2.right = parse_operand(pt, st);
          np1 = np2;
        
        } else if (tp->id == TK_DIV) {
            
	          scan_table_accept(st, TK_ANY);
	          np2 = parse_node_new(pt);
	          np2->type = EX_OPER2;
	          np2->oper2.oper = OP_DIV;
	          np2->oper2.left = np1;
	          np2->oper2.right = parse_operand(pt, st);
	          np1 = np2;
        } else if (tp->id == TK_PLUS) {

		    scan_table_accept(st, TK_ANY);
		    np2 = parse_node_new(pt);
		    np2->type = EX_OPER2;
		    np2->oper2.oper = OP_PLUS;
		    np2->oper2.left = np1;
		    np2->oper2.right = parse_operand(pt, st);
		   	np1 = np2;
		} else if (tp->id == TK_MINUS) {
			scan_table_accept(st, TK_ANY);
			np2 = parse_node_new(pt);
	    	np2->type = EX_OPER2;
	        np2->oper2.oper = OP_MINUS;
		    np2->oper2.left = np1;
		    np2->oper2.right = parse_operand(pt, st);
		   	np1 = np2;
		} else if (tp->id == TK_OR) {
			scan_table_accept(st, TK_ANY);
			np2 = parse_node_new(pt);
	    	np2->type = EX_OPER2;
	        np2->oper2.oper = OP_OR;
		    np2->oper2.left = np1;
		    np2->oper2.right = parse_operand(pt, st);
		   	np1 = np2;
		} else if (tp->id == TK_OR) {
			scan_table_accept(st, TK_ANY);
			np2 = parse_node_new(pt);
	    	np2->type = EX_OPER2;
	        np2->oper2.oper = OP_OR;
		    np2->oper2.left = np1;
		    np2->oper2.right = parse_operand(pt, st);
		   	np1 = np2;
		} else if (tp->id == TK_NOT) {
			scan_table_accept(st, TK_ANY);
			np2 = parse_node_new(pt);
	    	np2->type = EX_OPER2;
	        np2->oper2.oper = OP_NOT;
		    np2->oper2.left = np1;
		    np2->oper2.right = parse_operand(pt, st);
		   	np1 = np2;
		} else if (tp->id == TK_AND) {
			scan_table_accept(st, TK_ANY);
			np2 = parse_node_new(pt);
	    	np2->type = EX_OPER2;
	        np2->oper2.oper = OP_AND;
		    np2->oper2.left = np1;
		    np2->oper2.right = parse_operand(pt, st);
		   	np1 = np2;
		} else if (tp->id == TK_XOR) {
			scan_table_accept(st, TK_ANY);
			np2 = parse_node_new(pt);
	    	np2->type = EX_OPER2;
	        np2->oper2.oper = OP_XOR;
		    np2->oper2.left = np1;
		    np2->oper2.right = parse_operand(pt, st);
		   	np1 = np2;
		} else if (tp->id == TK_LSHIFT) {
			scan_table_accept(st, TK_ANY);
			np2 = parse_node_new(pt);
	    	np2->type = EX_OPER2;
	        np2->oper2.oper = OP_LSHIFT;
		    np2->oper2.left = np1;
		    np2->oper2.right = parse_operand(pt, st);
		   	np1 = np2;
		} else if (tp->id == TK_RSHIFT) {
			scan_table_accept(st, TK_ANY);
			np2 = parse_node_new(pt);
	    	np2->type = EX_OPER2;
	        np2->oper2.oper = OP_RSHIFT;
		    np2->oper2.left = np1;
		    np2->oper2.right = parse_operand(pt, st);
		   	np1 = np2;
		}
		 else {
		    break;
		}
		    }
	    
    	return np1;
}
int string_to_int(char *curr, int base){
	int length  = strlen(curr);
	int total;
	int val;
	int pv = 1;
	
	for(int i = length; i >=  0; i--){
		if((curr[i] >= '0' && curr[i] <= '9')){
		val = curr[i] - '0';
		total += (val * pv);
		pv *=  base;	
		}
	
	if( (curr[i] >= 'A' && curr[i] <= 'F') || curr[i] >= 'a' && curr[i] <= 'f'){
		if(curr[i] == 'A' || curr[i] == 'a'){
			val = 10;
		} else if(curr[i] == 'B' || curr[i] == 'b'){
			val = 11;
		} else if(curr[i] == 'c' || curr[i] == 'c'){
			val = 12;
		} else if(curr[i] == 'd' || curr[i] == 'D'){
			val = 13;
		} else if(curr[i] == 'e' || curr[i] == 'E'){
			val = 14;
		} else if(curr[i] == 'f' || curr[i] == 'F'){
			val = 15;
		}
		total +=(val *pv);
		pv *= base;
	}
	}
	return total;
}

// Parse operands, which are defined in the EBNF to be 
// integer literals or unary minus or expressions 
struct parse_node_st * parse_operand(struct parse_table_st *pt,
                                     struct scan_table_st *st) {
    struct scan_token_st *tp;
    struct parse_node_st *np1;

    /*
        TODO 
        add case for unary minus e.g. "-1 + 1"
        add case for operands which are expressions (i.e. begin with '(')
            (hint: recurse to parse_expression)
    */
		
    if (scan_table_accept(st, TK_INTLIT)) {
    	tp = scan_table_get(st, -1);
        np1 = parse_node_new(pt);
        np1->type = EX_INTVAL;
        np1->intval.value = string_to_int(tp->name, 10);
    } else if (scan_table_accept(st, TK_MINUS)){
    	tp = scan_table_get(st, -1);
    	np1 = parse_node_new(pt);
    	np1->type = EX_OPER1;
    	np1->oper1.oper  = OP_MINUS; 
    	np1->oper1.operand = parse_operand(pt,st);
    } else if (scan_table_accept(st, TK_NOT)){
        tp = scan_table_get(st, -1);
       	np1 = parse_node_new(pt);
       	np1->type = EX_OPER1;
        np1->oper1.oper  = OP_NOT; 
        np1->oper1.operand = parse_operand(pt,st);
    } else if ( scan_table_accept(st, TK_LPAREN)){
    	tp = scan_table_get(st, -1);
   		np1 = parse_expression(pt,st);
   		if (!scan_table_accept(st, TK_RPAREN)) {
   	        parse_error("Expecting EOT");        
   	    }   
    } else if (scan_table_accept(st, TK_BINLIT)) {
        	tp = scan_table_get(st, -1);
            np1 = parse_node_new(pt);
            np1->type = EX_INTVAL;
            np1->intval.value = string_to_int(tp->name, 2);
    } else if (scan_table_accept(st, TK_HEXLIT)) {
           	tp = scan_table_get(st, -1);
            np1 = parse_node_new(pt);
            np1->type = EX_INTVAL;
            np1->intval.value = string_to_int(tp->name, 16);
    }

       else {
        parse_error("Bad operand");
    }
  
    return np1;
}

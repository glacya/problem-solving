// BOJ 1918 - 후위 표기식
// 난이도: Gold II
// 푼 날짜: 2023-02-17
// https://www.acmicpc.net/problem/1918

use std::io;

#[derive(Clone)]
pub enum Term {
    Paren{
        lhs: Box<Term>,
        rhs: Box<Term>,
        operator: char,
    },
    
    Variable {
        name: String,
    }
}

#[derive(Clone)]
pub struct Pair {
    index: usize,
    operator: char,
}

impl Pair {
    pub fn pair(index: usize, operator: char) -> Pair {
        Pair{index, operator}
    }
}

impl Term {
    pub fn paren(lhs: &Term, rhs: &Term, operator: char) -> Term {
        Term::Paren {lhs: Box::new(lhs.clone()), rhs: Box::new(rhs.clone()), operator}
    }
    
    pub fn variable(name: &str) -> Term {
        Term::Variable {name: name.to_string()}
    }
}

fn substr(string: &str, start: usize, len: usize) -> String {
    let ss: String = string.chars().skip(start).take(len).collect();
    ss
}

fn string_to_terms(str_term: &str) -> Term {
    if str_term.len() == 1 {return Term::variable(str_term);}
    
    let mut paren_stack = 0;
    let mut operators: Vec<Pair> = Vec::new();
    
    for i in 0..str_term.len() {
        let digit = str_term.as_bytes()[i] as char;
        match digit {
            '+' | '-' | '*' | '/' => {
                if paren_stack == 0 {
                    operators.push(Pair::pair(i, digit));
                }
            },
            '(' => paren_stack += 1,
            ')' => paren_stack -= 1,
            _ => (),
        }
    }
    
    if operators.is_empty() {
        return string_to_terms(&substr(str_term, 1, str_term.len() - 2))
    }
    
    let mut new_terms: Vec<Term> = Vec::new();
    let mut eval_term: Term = Term::variable("asdfsadfa");
    let mut evaluated = false;
    
    for i in 0..operators.len() {
        let operator = operators[i].operator;
        match operator {
            '*' | '/' => {
                if evaluated {
                    let rhs_end = if i == operators.len() - 1 {str_term.len() - 1} else { operators[i + 1].index - 1} ;
                    let rhs = string_to_terms(&substr(str_term, operators[i].index + 1, rhs_end - operators[i].index));
                    eval_term = Term::paren(&eval_term, &rhs, operator);
                }
                else {
                    let lhs_start = if i == 0 { 0 } else { operators[i - 1].index + 1 };
                    let lhs = string_to_terms(&substr(str_term, lhs_start, operators[i].index - lhs_start));
                    let rhs_end = if i == operators.len() - 1 { str_term.len() - 1 } else { operators[i + 1].index - 1 };
                    let rhs = string_to_terms(&substr(str_term, operators[i].index + 1, rhs_end - operators[i].index));
                    eval_term = Term::paren(&lhs, &rhs, operator);
                    evaluated = true;
                }
            },
            '+' | '-' => {
                if evaluated {
                    new_terms.push(eval_term.clone());
                    evaluated = false;
                }
                else {
                    let lhs_start = if i == 0 { 0 } else { operators[i - 1].index + 1 };
                    let lhs = string_to_terms(&substr(str_term, lhs_start, operators[i].index - lhs_start));
                    new_terms.push(lhs);
                }
                
                if i == operators.len() - 1 {
                    new_terms.push(string_to_terms(&substr(str_term, operators[i].index + 1, str_term.len())))
                }
            },
            _ => todo!()
        }
    }
    
    if evaluated {
        new_terms.push(eval_term);
    }
    
    let mut final_term = new_terms[0].clone();
    let mut index = 0;
    let mut index_op = 0;
    
    while index + 1 < new_terms.len() {
        loop {
            let op = operators[index_op].operator;
            if op == '+' || op == '-' {
                break
            }
            index_op += 1;
        }
        final_term = Term::paren(&final_term, &new_terms[index + 1], operators[index_op].operator);
        index += 1;
        index_op += 1;
    }
    
    final_term
}

fn terms_to_postfix(term: Term) -> String {
    match term {
        Term::Paren{lhs, rhs, operator} => terms_to_postfix(*lhs) + &terms_to_postfix(*rhs) + &operator.to_string(),
        Term::Variable {name} => name
    }
}

fn main() {
    let mut infix = String::new();
    io::stdin().read_line(&mut infix).expect("oh");
    
    let term = string_to_terms(&infix.trim());
    let postfix = terms_to_postfix(term);
    println!("{postfix}");
}

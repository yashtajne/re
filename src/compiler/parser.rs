use crate::compiler::{Compiler, token::Token};

#[derive(Clone, Copy)]
pub enum Symbol {
    Invalid,
    Plus,
    Minus,
    DoublePlus,
    DoubleMinus,
}

#[derive(Clone)]
pub enum Literal {
    Invalid,
    String(String),
}

#[allow(dead_code)] // <----------------------- Dead Code Allowed

pub enum Expr {
    Invalid,
    Symbolic { symbol: Symbol, operands: Vec<Expr> },
    Int(i64),
    Float(f64),
    String(String)
}

impl Symbol {
    pub fn get(&self) -> &str{
        match self {
            Self::Plus => "+",
            Self::Minus => "-",
            _ => todo!("Implement Printing Symbols"),
        }
    }
}

impl Expr {
    pub fn print(&self) {
        self.print_tree("", true);
    }

    fn print_tree(&self, prefix: &str, is_last: bool) {
        let branch = if is_last { "└─" } else { "├─" };

        match self {
            Expr::Symbolic { symbol, operands } => {
                println!("{prefix}{branch}[Expr Symbolic] {}", symbol.get());

                let next_prefix = format!("{prefix}{}", if is_last { "  " } else { "│ " });
                for (i, op) in operands.iter().enumerate() {
                    op.print_tree(&next_prefix, i + 1 == operands.len());
                }
            }
            Expr::String(s) => println!("{prefix}{branch}[Expr String] \"{s}\""),
            Expr::Int(v) => println!("{prefix}{branch}[Expr Int] {v}"),
            Expr::Float(v) => println!("{prefix}{branch}[Expr Float] {v}"),
            Expr::Invalid => println!("{prefix}{branch}[Expr Invalid]"),
        }
    }

    // pub fn print(&self, indents: usize) {
    //     match self {
    //         Expr::String(ls) => println!("[Expr String]: \"{}\"", ls),
    //         Expr::Int(li) => println!("[Expr Int] {}i64", li),
    //         Expr::Float(lf) => println!("[Expr Float] {}f64", lf),
    //         Expr::Symbolic { symbol, operands } => {
    //             println!("[Expr Symbolic]");
    //             print!(" ┃ Symbol: {}", symbol.get());
    //             println!();
    //             for i in 0..operands.len() {
    //                 print!(" {} {} ", "│".repeat(indents), i);
    //                 operands[i].print(indents + 1);
    //             }
    //         },
    //         Expr::Invalid => todo!("Implement Printing Expr!?")
    //     }
    // }

    pub fn parse(compiler: &mut Compiler) -> Expr {
        if matches!(compiler.current_token, Token::Symbol(_)) {
            let symbol = match &compiler.current_token {
                Token::Symbol(s) => s.clone(),
                _ => { Symbol::Invalid }
            };

            let mut operands = Vec::new();

            compiler.advance();
            operands.push(Self::parse(compiler));
            operands.push(Self::parse(compiler));

            Expr::Symbolic { symbol, operands }
        }

        else
        if matches!(compiler.current_token, Token::Literal(_)) {
            let literal = match &compiler.current_token {
                Token::Literal(l) => l.clone(),
                _ => { Literal::Invalid }
            };

            match literal {
                Literal::String(ls) => {
                    compiler.advance();
                    Expr::String(ls)
                },
                _ => {
                    compiler.advance();
                    Expr::Invalid
                }
            }
        }

        else { Expr::Invalid }
    }
}
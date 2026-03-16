use crate::compiler::token::{Token};

#[derive(Debug)]
pub enum Expr {
    Invalid,

    Symbolic {
        symbol: Token,
        operands: Vec<Expr>
    },

    Identifier(String),

    IntLiteral(i64),
    StringLiteral(String),
}


impl Expr {
    pub fn print(&self) {
        match self {
            Self::Invalid => print!("- Expr Invalid"),

            Self::Identifier(l) => {
                println!("[Expr Identifier]");
                println!("{:#?}\n", l);
            },

            Self::IntLiteral(i) => {
                println!("[Expr IntLiteral]");
                println!("{:#?}\n", i);
            },

            Self::StringLiteral(s) => {
                println!("[Expr StringLiteral]");
                println!("{:#?}\n", s);
            },

            s @ Self::Symbolic { .. } => {
                println!("[Expr Symbolic]");
                println!("{:#?}", s);
            }
        }
    }
}
use crate::compiler::token::{self, Token};

#[derive(Debug)]
pub enum Expr {
    Invalid,

    Symbolic {
        symbol: Token,
        operands: Vec<Expr>
    },

    Identifier(String),
    StringLiteral(String),
}


impl Expr {
    pub fn print(&self) {
        match self {
            Self::Invalid => print!("- Expr Invalid"),

            Self::Identifier(l) => {
                println!("- Expr Identifier");
                println!("{:#?}\n", l);
            },
            Self::StringLiteral(s) => {
                println!("- Expr StringLiteral");
                println!("{:#?}\n", s);
            },

            s @ Self::Symbolic { .. } => {
                println!("- Expr Symbolic");
                println!("{:#?}", s)
            }
        }
    }
}
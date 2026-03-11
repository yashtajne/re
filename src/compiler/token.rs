use crate::compiler::parser::{Literal, Symbol};


pub enum Token {
    Eof,
    Invalid,

    SemiColon,

    Symbol(Symbol),
    Literal(Literal),

    TypeName(String),
    Identifier(String),
}

#[allow(dead_code)]
impl Token {
    pub fn print(&self) {
        match self {
            Token::Invalid => print!("Token -> Invalid"),
            Token::Eof => print!("Token -> End Of File"),

            Token::SemiColon => print!("Token -> SemiColon: ;"),

            Token::Symbol(s) => match s {
                Symbol::Plus => print!("Token -> Symbol -> Plus: +"),
                Symbol::Minus => print!("Token -> Symbol -> Minus: -"),
                Symbol::DoublePlus => print!("Token -> Symbol -> DoublePlus: ++"),
                Symbol::DoubleMinus => print!("Token -> Symbol -> DoubleMinus: --"),
                Symbol::Invalid => print!("Token -> Symbol -> Invalid")
            }

            Token::Literal(l) => match l {
                Literal::String(ls) => print!("Token -> Literal -> String: \"{}\"", ls),
                Literal::Invalid => print!("Token -> Literal -> Invalid")
            }

            Token::TypeName(s) => print!("Token -> TypeName: `{}`", s),
            Token::Identifier(s) => print!("Token -> Identifier: `{}`", s),
            // _ => todo!("Implement token printing! for other tokens")
        }
    }
}
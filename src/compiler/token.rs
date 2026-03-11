

pub enum Token {
    Eof,
    Invalid,

    Plus,
    Minus,

    DoublePlus,
    DoubleMinus,

    Identifier(String),
}


impl Token {
    pub fn print(&self) {
        match self {
            Token::Invalid => print!("Invalid"),
            Token::Eof => print!("End Of File"),

            Token::Plus => print!("Plus +"),
            Token::Minus => print!("Minus -"),

            Token::DoublePlus => print!("DoublePlus ++"),
            Token::DoubleMinus => print!("DoubleMinus --"),

            Token::Identifier(s) => print!("Identifier `{}`", s),
            // _ => todo!("Implement token printing! for other tokens")
        }
    }
}
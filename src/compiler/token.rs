
#[derive(Debug, Clone)]
pub enum Token {
    Eof,
    Invalid,

    SemiColon,

    Plus,
    Minus,

    OpenRoundBracket,
    CloseRoundBracket,

    StringLiteral(String),
    IntLiteral(i64),
    // FloatLiteral,

    TypeName(String),
    Identifier(String),
}

impl Token {
    pub fn print(&self) {
        print!(
            "{}",
            match self {
                Token::Invalid => format!("{:<20}", "Invalid"),
                Token::Eof => format!("{:<20}", "Eof"),
                Token::SemiColon => format!("{:<20} {}", "SemiColon", ";"),
                Token::Plus => format!("{:<20} {}", "Plus", "+"),
                Token::Minus => format!("{:<20} {}", "Minus", "-"),
                Token::OpenRoundBracket => format!("{:<20} {}", "OpenRoundBracket", "("),
                Token::CloseRoundBracket => format!("{:<20} {}", "CloseRoundBracket", ")"),
                Token::StringLiteral(s) => format!("{:<20} {}", "StringLiteral", s),
                Token::IntLiteral(s) => format!("{:<20} {}", "IntLiteral", s),
                Token::TypeName(t) => format!("{:<20} {}", "TypeName", t),
                Token::Identifier(i) => format!("{:<20} {}", "Identifier", i),
            }
        );
    }
}


#[allow(unused)]
#[derive(Debug, Clone)]
pub enum Token {
    Eof { row: usize, col: usize },
    Invalid { row: usize, col: usize },

    SemiColon { row: usize, col: usize },

    Plus { row: usize, col: usize },
    Minus { row: usize, col: usize },

    OpenRoundBracket { row: usize, col: usize },
    CloseRoundBracket { row: usize, col: usize },

    StringLiteral { row: usize, col: usize, value: String },
    IntLiteral { row: usize, col: usize, value: i64 },
    // FloatLiteral,

    TypeName { row: usize, col: usize, value: String },
    Identifier { row: usize, col: usize, value: String },
}

impl Token {
    pub fn print(&self) {
        println!("{:#?}", self);
    }
}
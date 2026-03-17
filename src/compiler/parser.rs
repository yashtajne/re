use std::fmt::format;

use crate::compiler::{Compiler, token::Token};

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


impl Compiler {
    fn advance(&mut self) {
        let next_token = self.next();
        self.current_token = next_token;
    }

    pub fn parse_expr(&mut self) -> Expr {
        if let Token::OpenRoundBracket = self.current_token {
            let opened_position = self.get_current_position();

            self.advance();
            let expr = self.parse_expr();

            if !matches!(self.current_token, Token::CloseRoundBracket) {

                self.error(format!("Error: forgot to close round bracket? Opened at [{}:{}]", opened_position.0, opened_position.1));
            }

            self.advance();
            return expr;
        }

        if matches!(&self.current_token,
            Token::Plus |
            Token::Minus
        ) {
            let symbol = self.current_token.clone();
            let mut operands = Vec::new();

            self.advance();

            while !matches!(self.current_token,
                Token::CloseRoundBracket |
                Token::SemiColon |
                Token::Eof
            ) {
                operands.push(self.parse_expr());
            }

            return Expr::Symbolic { symbol, operands };
        }

        let expr = match &self.current_token {
            Token::StringLiteral(s) => Expr::StringLiteral(s.clone()),
            Token::IntLiteral(i) => Expr::IntLiteral(i.clone()),
            Token::Identifier(i) => Expr::Identifier(i.clone()),
            _ => Expr::Invalid
        };

        self.advance();
        expr
    }
}

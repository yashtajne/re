use std::io::{BufRead};

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


impl<R: BufRead> Compiler<R> {
    pub fn parse_expr(&mut self) -> Expr {
        if let Token::CloseRoundBracket{..} = self.current_token {
            let position = self.get_current_position();
            self.error(format!(
                "Unnessesary close round bracket found here! [{}:{}]",
                position.0, position.1
            ));
        }

        // expr inside brackets
        if let Token::OpenRoundBracket{..} = self.current_token {
            let opened_position = self.get_current_position();

            self.next();
            let expr = self.parse_expr();

            if !matches!(self.current_token, Token::CloseRoundBracket{..}) {
                self.error(format!("forgot to close round bracket? Opened at [{}:{}]", opened_position.0, opened_position.1));
            }

            self.next();
            return expr;
        }

        else // Symbolic expr
        if matches!(&self.current_token,
            Token::Plus{..} |
            Token::Minus{..}
        ) {
            let symbol = self.current_token.clone();
            let mut operands = Vec::new();

            self.next();

            while !matches!(self.current_token,
                Token::CloseRoundBracket{..} |
                Token::Eof{..}
            ) {
                operands.push(self.parse_expr());
            }

            return Expr::Symbolic { symbol, operands };
        }

        else { // Leaf expr
            let expr = match &self.current_token {
                Token::StringLiteral{ value, .. } => Expr::StringLiteral(value.clone()),
                Token::IntLiteral{ value, .. } => Expr::IntLiteral(value.clone()),
                Token::Identifier{ value, .. } => Expr::Identifier(value.clone()),
                _ => Expr::Invalid
            };

            self.next();
            expr
        }
    }
}

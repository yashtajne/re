pub mod token;
pub mod lexer;
pub mod parser;

use crate::compiler::{parser::Expr, token::Token};
use std::{fs::File, io::{BufReader, Seek}};

pub struct Compiler {
    file: BufReader<File>,
    row: usize,
    bol: usize,
    current_token: Token,
    types: Vec<String>,
}

impl Compiler {
    pub fn create(file: &str) -> Self {
        let open_file = File::open(file);
        let file = match open_file {
            Ok(f) => BufReader::new(f),
            Err(e) => panic!("Error: {}", e),
        };

        let mut compiler = Self{
            row: 1, bol: 0,
            current_token: Token::Invalid,
            file: file, types: vec!["int".to_string()]
        };

        compiler.current_token = lexer::next(&mut compiler);
        compiler
    }

    fn reset(&mut self) {
        self.row = 1;
        self.bol = 0;

        self.file.rewind().unwrap_or_else(|e| {
            panic!("Error: Unable to rewind BufReader.. Because {}", e)
        });

        self.types.clear();
        self.current_token = lexer::next(self);
    }

    fn get_current_position(&mut self) -> (usize, usize) {
        (self.row, (self.file.stream_position().unwrap() as usize) - self.bol + 1)
    }

    pub fn compile(&mut self) {
        println!();
        println!("- - - - - [ Tokenization ] - - - - - -");
        println!();

        while !matches!(self.current_token, Token::Eof) {
            let current_position = self.get_current_position();
            print!("[{:>2}:{:<3}] ", current_position.0, current_position.1);
            self.current_token.print();
            println!();

            self.current_token = lexer::next(self);
        }
        self.reset();

        println!();
        println!("- - - - - [ Parsing ] - - - - - -");
        println!();

        let mut expr = self.parse_expr();
        while !matches!(expr, Expr::Invalid) {
            expr.print();
            expr = self.parse_expr();
        }

        println!();
    }

    fn advance(&mut self) {
        let next_token = lexer::next(self);
        self.current_token = next_token;
    }

    fn parse_expr(&mut self) -> parser::Expr {

        if let Token::OpenRoundBracket = self.current_token {
            self.advance();
            return self.parse_expr()
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

            self.advance();

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
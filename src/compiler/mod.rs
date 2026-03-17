pub mod token;
pub mod lexer;
pub mod parser;
pub mod errors;

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
    pub fn create_for_file(file: &str) -> Self {
        let open_file = File::open(file);
        let file = match open_file {
            Ok(f) => BufReader::new(f),
            Err(e) => panic!("Error: {}", e),
        };

        let mut compiler = Self{
            row: 1, bol: 0,
            current_token: Token::Invalid{ row: 0, col: 0 },
            file: file, types: vec!["int".to_string()]
        };

        compiler.next();
        compiler
    }

    pub fn list_all_tokens(file: &str) {
        let mut compiler = Self::create_for_file(file);

        while !matches!(compiler.current_token, Token::Eof{..} | Token::Invalid{..}) {
            let current_position = compiler.get_current_position();
            print!("[{}:{}]", current_position.0, current_position.1);
            compiler.current_token.print();
            println!();
            compiler.next();
        }
    }

    pub fn get_current_position(&mut self) -> (usize, usize) {
        (self.row, (self.file.stream_position().unwrap() as usize) - self.bol + 1)
    }

    pub fn compile(&mut self) {
        let mut expr = self.parse_expr();
        while !matches!(expr, Expr::Invalid) {
            expr.print();
            expr = self.parse_expr();
        }

        println!();
    }
}
pub mod token;
pub mod lexer;
pub mod parser;

use crate::compiler::token::{Token};
use std::{fs::File, io::{BufReader, Seek, SeekFrom}};

pub struct Compiler {
    file: BufReader<File>,
    row: u64,
    bol: u64,
    current_token: Token,
    types: Vec<String>,
}

#[allow(dead_code)]
impl Compiler {
    pub fn create(file: &str) -> Self {
        let open_file = File::open(file);
        let file = match open_file {
            Ok(f) => BufReader::new(f),
            Err(e) => panic!("Error: {}", e),
        };

        let mut compiler = Self{
            row: 1, bol: 0, current_token: Token::Invalid,
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

    fn advance(&mut self) {
        let next_token = lexer::next(self);
        self.current_token = next_token;
    }

    fn get_current_position(&mut self) -> (u64, u64) {
        (self.row, self.file.seek(SeekFrom::Current(0)).unwrap_or_else(|e| {
            panic!("Error: failed to seek current position in buffered reader... {}", e);
        }) - self.bol)
    }

    pub fn compile(&mut self) {
        println!();
        println!("- - - - - [ Tokenization ] - - - - - -");
        println!();

        self.print_all_tokens();
        self.reset();

        println!();
        println!("- - - - - [ Parsing ] - - - - - -");
        println!();

        while !matches!(self.current_token, Token::Eof) {
            parser::Expr::parse(self).print();
        }
    }

    pub fn print_all_tokens(&mut self) {
        while !matches!(self.current_token, Token::Eof) {
            let current_position = self.get_current_position();

            print!("[{}:{}] ", current_position.0, current_position.1);
            self.current_token.print();
            println!();

            self.current_token = lexer::next(self);
        }
        self.reset();
    }
}
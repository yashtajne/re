pub mod token;
pub mod lexer;
pub mod parser;

use crate::compiler::token::{Token};
use std::{fs::File, io::{BufReader, Seek, SeekFrom}};

pub struct Compiler {
    row: u64,
    bol: u64,
    file: BufReader<File>,
    types: Vec<String>,
}

impl Compiler {
    pub fn create(file: &str) -> Self {
        let open_file = File::open(file);
        let file = match open_file {
            Ok(f) => BufReader::new(f),
            Err(e) => panic!("Error: {}", e),
        };

        Self{
            row: 1, bol: 0,
            file: file, types: vec!["int".to_string()]
        }
    }

    fn get_current_position(&mut self) -> (u64, u64) {
        (self.row, self.file.seek(SeekFrom::Current(0)).unwrap_or_else(|e| {
            panic!("Error: failed to seek current position in buffered reader... {}", e);
        }) - self.bol)
    }

    pub fn compile(&mut self) {
        let mut token = lexer::next(self);
        loop {
            let pos = self.get_current_position();
            print!("[{}:{}] ", pos.0, pos.1);
            token.print();
            println!();
            match token {
                Token::Eof => break,
                _ => {
                    token = lexer::next(self);
                }
            }
        }
    }
}
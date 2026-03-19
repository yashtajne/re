pub mod token;
pub mod lexer;
pub mod parser;
pub mod errors;

use crate::compiler::{parser::Expr, token::Token};
use std::{fs::File, io::{self, BufRead, BufReader, Cursor, Write}};


pub struct Compiler<R: BufRead> {
    file: R,
    pos: usize,
    row: usize,
    bol: usize,
    peeked: Option<char>,
    current_token: Token,
    types: Vec<String>,
}


impl<R: BufRead> Compiler<R> {
    pub fn create(file: R) -> Self {
        let mut compiler = Self{
            pos: 0, row: 1, bol: 0,
            peeked: None,
            current_token: Token::Invalid{ row: 0, col: 0 },
            file: file, types: vec!["int".to_string()]
        };

        compiler.next();
        compiler
    }

    pub fn create_for_file(file: &str) -> Compiler<BufReader<File>> {
        let open_file = File::open(file);
        let file = match open_file {
            Ok(f) => f,
            Err(e) => panic!("Error: {}", e),
        };

        Compiler::create(BufReader::new(file))
    }

    pub fn list_all_tokens(file: &str) {
        let mut compiler = Self::create_for_file(file);

        while !matches!(compiler.current_token, Token::Eof{..}) {
            // let current_position = compiler.get_current_position();
            // print!("[{}:{}]", current_position.0, current_position.1);
            compiler.current_token.print();
            compiler.next();
        }
        compiler.current_token.print();
    }

    pub fn get_current_position(&mut self) -> (usize, usize) {
        // let stream_position = self.file.stream_position()
        //     .expect("Failed to get stream position!") as usize;
        // (self.row, stream_position - self.bol + 1)
        (self.row, self.pos - self.bol + 1)
    }

    pub fn compile(&mut self) {
        let mut expr = self.parse_expr();
        while !matches!(expr, Expr::Invalid) {
            expr.print();
            expr = self.parse_expr();
        }

        println!();
    }

    pub fn shell() {
        let mut input = String::new();
        let mut row = 0;

        loop {
            // --- Prompt ---

            print!("re[{}:0]> ", row);
            io::stdout().flush().expect("Failed to flush stdout!");

            // --- Reading Input ---

            input.clear();
            io::stdin()
                .read_line(&mut input)
                .expect("Failed to read_line() from stdin!");
            row += 1;

            // --- Compilation ---

            #[allow(irrefutable_let_patterns)]
            if let ref source = input
            && !source.trim().is_empty() {
                println!("Source: {}", source);
                let mut compiler = Compiler::create(Cursor::new(source));

                // println!("- - - - - [ Tokenization ] - - - - - -");
                // while !matches!(compiler.current_token, Token::Eof{..}) {
                //     compiler.current_token.print();
                //     compiler.next();
                // }
                // println!();

                println!("- - - - - [ Parsing ] - - - - - -");
                compiler.compile();
            }
            io::stdout().flush().expect("Failed to flush stdout!");
        }
    }
}

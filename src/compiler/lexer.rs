use std::io::{BufRead, Seek, SeekFrom};
use crate::compiler::parser::{Literal, Symbol};
use crate::compiler::token::{Token};
use crate::compiler::Compiler;

pub fn next(compiler: &mut Compiler) -> Token {
    loop {
        let (first, second) = {
            let b = compiler.file.fill_buf().unwrap_or_else(|e| panic!("Error: failed to read buffered input... {}", e));
            if b.is_empty() {
                return Token::Eof;
            }
            (b[0] as char, b.get(1).copied().map(|x| x as char))
        };

        if first.is_whitespace() {
            compiler.file.consume(1);
            if first == '\n' {
                compiler.row += 1;
                compiler.bol = compiler.file.seek(SeekFrom::Current(0)).unwrap_or_else(|e| {
                    panic!("Error: failed to seek current position in buffered reader... {}", e);
                });
            }
            continue;
        }

        if first.is_alphabetic() || first == '_' {
            let len = {
                let b = compiler.file.fill_buf().unwrap_or_else(|e| panic!("Error: failed to read buffered input... {}", e));
                let mut i = 0;
                while i < b.len() {
                    let c = b[i] as char;
                    if c.is_alphanumeric() || c == '_' { i += 1; } else { break; }
                }
                i
            };

            let lexeme = {
                let b = compiler.file.fill_buf().unwrap_or_else(|e| panic!("Error: failed to read buffered input... {}", e));
                String::from_utf8_lossy(&b[..len]).to_string()
            };

            compiler.file.consume(len);

            if compiler.types.contains(&lexeme) {
                return Token::TypeName(lexeme);
            }

            return Token::Identifier(lexeme);
        }

        if first == '"' {
            let (content_end, consumed) = {
                let b = compiler.file.fill_buf().unwrap_or_else(|e| {
                    panic!("Error: failed to read buffered input... {}", e)
                });

                let mut i = 1;
                while i < b.len() && b[i] as char != '"' {
                    i += 1;
                }

                let consumed = if i < b.len() { i + 1 } else { i };
                (i, consumed)
            };

            let lexeme = {
                let b = compiler.file.fill_buf().unwrap_or_else(|e| {
                    panic!("Error: failed to read buffered input... {}", e)
                });
                String::from_utf8_lossy(&b[1..content_end]).to_string()
            };

            compiler.file.consume(consumed);
            return Token::Literal(Literal::String(lexeme));
        }


        let (tok, n) = match (first, second) {
            ('+', Some('+')) => (Token::Symbol(Symbol::DoublePlus), 2),
            ('-', Some('-')) => (Token::Symbol(Symbol::DoubleMinus), 2),
            ('+', _) => (Token::Symbol(Symbol::Plus), 1),
            ('-', _) => (Token::Symbol(Symbol::Minus), 1),
            (';', _) => (Token::SemiColon, 1),
            _ => (Token::Invalid, 1),
        };

        compiler.file.consume(n);
        return tok;
    }
}

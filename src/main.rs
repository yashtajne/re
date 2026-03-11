mod compiler;
use std::{env, fs};
use compiler::Compiler;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        panic!("no input files");
    }

    let file_path = fs::canonicalize(&args[1]).unwrap();
    let file = match file_path.to_str() {
        Some(f) => f,
        None => panic!("Error: file not found")
    };

    let mut compiler = Compiler::create(file);
    compiler.compile();
}

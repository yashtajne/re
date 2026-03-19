mod compiler;
use std::{env, fs::{self, File}, io::{BufReader, Cursor}};
use compiler::Compiler;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        // panic!("no input files");
        Compiler::<BufReader<Cursor<String>>>::shell();
    }

    let file_path = fs::canonicalize(&args[1]).unwrap();
    let file = match file_path.to_str() {
        Some(f) => f,
        None => panic!("Error: file not found")
    };

    let mut compiler = Compiler::<BufReader<File>>::create_for_file(file);
    println!();
    println!("- - - - - [ Tokenization ] - - - - - -");
    println!();

    Compiler::<BufReader<File>>::list_all_tokens(file);

    println!();
    println!("- - - - - [ Parsing ] - - - - - -");
    println!();

    compiler.compile();
}

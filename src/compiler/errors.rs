use std::io::{BufRead};
use crate::compiler::Compiler;


impl<R: BufRead> Compiler<R> {
    pub fn error(&mut self, msg: String) {
        let current_position = self.get_current_position();
        print!("[{:>2}:{:<3}] Error: ", current_position.0, current_position.1);
        print!("{}", msg);
        println!();
        panic!();
    }
}
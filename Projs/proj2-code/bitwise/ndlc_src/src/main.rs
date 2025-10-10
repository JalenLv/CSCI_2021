// SPDX-License-Identifier: GPL-3.0-or-later
// Author: John Kolb <jhkolb@umn.edu>

use std::{env, process::ExitCode};

use ndlc::*;

fn main() -> ExitCode {
    let args: Vec<String> = env::args().collect();
    if args.len() < 3 {
        println!("Usage: {} <puzzle_spec_json_file> <c_file>", args[0]);
        return ExitCode::from(1);
    }

    match check_file(&args[1], &args[2]) {
        Ok(true) => {
            println!("No violations found");
            ExitCode::from(0)
        }
        Ok(false) => ExitCode::from(1),
        Err(e) => {
            println!("{}", e);
            ExitCode::from(1)
        }
    }
}

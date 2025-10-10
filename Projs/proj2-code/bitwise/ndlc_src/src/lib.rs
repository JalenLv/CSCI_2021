// Author: John Kolb <jhkolb@umn.edu>
// SPDX-License-Identifier: GPL-3.0-or-later
// Version 0.0.1
// This is probably terrible Rust code
// Rewrite it someday to be more functional

use std::{
    collections::{HashMap, HashSet},
    error::Error,
    io::BufRead as _,
};

use lang_c::visit::Visit as _;
use regex::Regex;

use serde::{Deserialize, Serialize};
use serde_with::{serde_as, DisplayFromStr};

#[derive(PartialEq, Eq, Hash, Debug)]
enum BinaryOperator {
    Index,
    Multiply,
    Divide,
    Modulo,
    Plus,
    Minus,
    ShiftLeft,
    ShiftRight,
    Less,
    Greater,
    LessOrEqual,
    GreaterOrEqual,
    Equals,
    NotEquals,
    BitwiseAnd,
    BitwiseXor,
    BitwiseOr,
    LogicalAnd,
    LogicalOr,
    Assign,
    AssignMultiply,
    AssignDivide,
    AssignModulo,
    AssignPlus,
    AssignMinus,
    AssignShiftLeft,
    AssignShiftRight,
    AssignBitwiseAnd,
    AssignBitwiseXor,
    AssignBitwiseOr,
}

impl std::fmt::Display for BinaryOperator {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        match self {
            Self::Index => write!(f, "[]"),
            Self::Multiply => write!(f, "*"),
            Self::Divide => write!(f, "/"),
            Self::Modulo => write!(f, "%"),
            Self::Plus => write!(f, "+"),
            Self::Minus => write!(f, "-"),
            Self::ShiftLeft => write!(f, "<<"),
            Self::ShiftRight => write!(f, ">>"),
            Self::Less => write!(f, "<"),
            Self::Greater => write!(f, ">"),
            Self::LessOrEqual => write!(f, "<="),
            Self::GreaterOrEqual => write!(f, ">="),
            Self::Equals => write!(f, "=="),
            Self::NotEquals => write!(f, "!="),
            Self::BitwiseAnd => write!(f, "&"),
            Self::BitwiseXor => write!(f, "^"),
            Self::BitwiseOr => write!(f, "|"),
            Self::LogicalAnd => write!(f, "&&"),
            Self::LogicalOr => write!(f, "||"),
            Self::Assign => write!(f, "="),
            Self::AssignMultiply => write!(f, "*="),
            Self::AssignDivide => write!(f, "/="),
            Self::AssignModulo => write!(f, "%="),
            Self::AssignPlus => write!(f, "+="),
            Self::AssignMinus => write!(f, "-="),
            Self::AssignShiftLeft => write!(f, "<<="),
            Self::AssignShiftRight => write!(f, ">>="),
            Self::AssignBitwiseAnd => write!(f, "&="),
            Self::AssignBitwiseXor => write!(f, "^="),
            Self::AssignBitwiseOr => write!(f, "|="),
        }
    }
}

struct BinOpParseError;

impl std::fmt::Display for BinOpParseError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "BinOpParseError")
    }
}

impl std::str::FromStr for BinaryOperator {
    type Err = BinOpParseError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "[]" => Ok(Self::Index),
            "*" => Ok(Self::Multiply),
            "/" => Ok(Self::Divide),
            "%" => Ok(Self::Modulo),
            "+" => Ok(Self::Plus),
            "-" => Ok(Self::Minus),
            "<<" => Ok(Self::ShiftLeft),
            ">>" => Ok(Self::ShiftRight),
            "<" => Ok(Self::Less),
            ">" => Ok(Self::Greater),
            "<=" => Ok(Self::LessOrEqual),
            ">=" => Ok(Self::GreaterOrEqual),
            "==" => Ok(Self::Equals),
            "!=" => Ok(Self::NotEquals),
            "&" => Ok(Self::BitwiseAnd),
            "^" => Ok(Self::BitwiseXor),
            "|" => Ok(Self::BitwiseOr),
            "&&" => Ok(Self::LogicalAnd),
            "||" => Ok(Self::LogicalOr),
            "=" => Ok(Self::Assign),
            "*=" => Ok(Self::AssignMultiply),
            "/=" => Ok(Self::AssignDivide),
            "%=" => Ok(Self::AssignModulo),
            "+=" => Ok(Self::AssignPlus),
            "-=" => Ok(Self::AssignMinus),
            "<<=" => Ok(Self::AssignShiftLeft),
            ">>=" => Ok(Self::AssignShiftRight),
            "&=" => Ok(Self::AssignBitwiseAnd),
            "^=" => Ok(Self::AssignBitwiseXor),
            "|=" => Ok(Self::AssignBitwiseOr),
            _ => Err(BinOpParseError),
        }
    }
}

impl std::convert::From<&lang_c::ast::BinaryOperator> for BinaryOperator {
    fn from(value: &lang_c::ast::BinaryOperator) -> Self {
        match value {
            lang_c::ast::BinaryOperator::Index => Self::Index,
            lang_c::ast::BinaryOperator::Multiply => Self::Multiply,
            lang_c::ast::BinaryOperator::Divide => Self::Divide,
            lang_c::ast::BinaryOperator::Modulo => Self::Modulo,
            lang_c::ast::BinaryOperator::Plus => Self::Plus,
            lang_c::ast::BinaryOperator::Minus => Self::Minus,
            lang_c::ast::BinaryOperator::ShiftLeft => Self::ShiftLeft,
            lang_c::ast::BinaryOperator::ShiftRight => Self::ShiftRight,
            lang_c::ast::BinaryOperator::Less => Self::Less,
            lang_c::ast::BinaryOperator::Greater => Self::Greater,
            lang_c::ast::BinaryOperator::LessOrEqual => Self::LessOrEqual,
            lang_c::ast::BinaryOperator::GreaterOrEqual => Self::GreaterOrEqual,
            lang_c::ast::BinaryOperator::Equals => Self::Equals,
            lang_c::ast::BinaryOperator::NotEquals => Self::NotEquals,
            lang_c::ast::BinaryOperator::BitwiseAnd => Self::BitwiseAnd,
            lang_c::ast::BinaryOperator::BitwiseXor => Self::BitwiseXor,
            lang_c::ast::BinaryOperator::BitwiseOr => Self::BitwiseOr,
            lang_c::ast::BinaryOperator::LogicalAnd => Self::LogicalAnd,
            lang_c::ast::BinaryOperator::LogicalOr => Self::LogicalOr,
            lang_c::ast::BinaryOperator::Assign => Self::Assign,
            lang_c::ast::BinaryOperator::AssignMultiply => Self::AssignMultiply,
            lang_c::ast::BinaryOperator::AssignDivide => Self::AssignDivide,
            lang_c::ast::BinaryOperator::AssignModulo => Self::AssignModulo,
            lang_c::ast::BinaryOperator::AssignPlus => Self::AssignPlus,
            lang_c::ast::BinaryOperator::AssignMinus => Self::AssignMinus,
            lang_c::ast::BinaryOperator::AssignShiftLeft => Self::AssignShiftLeft,
            lang_c::ast::BinaryOperator::AssignShiftRight => Self::AssignShiftRight,
            lang_c::ast::BinaryOperator::AssignBitwiseAnd => Self::AssignBitwiseAnd,
            lang_c::ast::BinaryOperator::AssignBitwiseXor => Self::AssignBitwiseXor,
            lang_c::ast::BinaryOperator::AssignBitwiseOr => Self::AssignBitwiseOr,
        }
    }
}

#[derive(PartialEq, Eq, Hash, Debug)]
enum UnaryOperator {
    PostIncrement,
    PreIncrement,
    PostDecrement,
    PreDecrement,
    Address,
    Indirection,
    Plus,
    Minus,
    Complement,
    Negate,
}

impl std::fmt::Display for UnaryOperator {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::PostIncrement | Self::PreIncrement => write!(f, "++"),
            Self::PostDecrement | Self::PreDecrement => write!(f, "--"),
            Self::Address => write!(f, "&"),
            Self::Indirection => write!(f, "*"),
            Self::Plus => write!(f, "+"),
            Self::Minus => write!(f, "-"),
            Self::Complement => write!(f, "~"),
            Self::Negate => write!(f, "!"),
        }
    }
}

struct UnaryOpParseError;

impl std::fmt::Display for UnaryOpParseError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "UnaryOpParseError")
    }
}

impl std::str::FromStr for UnaryOperator {
    type Err = UnaryOpParseError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "++" => Ok(Self::PostIncrement),
            "--" => Ok(Self::PostDecrement),
            "&" => Ok(Self::Address),
            "*" => Ok(Self::Indirection),
            "+" => Ok(Self::Plus),
            "-" => Ok(Self::Minus),
            "~" => Ok(Self::Complement),
            "!" => Ok(Self::Negate),
            _ => Err(UnaryOpParseError),
        }
    }
}

impl std::convert::From<&lang_c::ast::UnaryOperator> for UnaryOperator {
    fn from(value: &lang_c::ast::UnaryOperator) -> Self {
        match value {
            lang_c::ast::UnaryOperator::PostIncrement => Self::PostIncrement,
            lang_c::ast::UnaryOperator::PostDecrement => Self::PostDecrement,
            lang_c::ast::UnaryOperator::PreIncrement => Self::PreIncrement,
            lang_c::ast::UnaryOperator::PreDecrement => Self::PreDecrement,
            lang_c::ast::UnaryOperator::Address => Self::Address,
            lang_c::ast::UnaryOperator::Indirection => Self::Indirection,
            lang_c::ast::UnaryOperator::Plus => Self::Plus,
            lang_c::ast::UnaryOperator::Minus => Self::Minus,
            lang_c::ast::UnaryOperator::Complement => Self::Complement,
            lang_c::ast::UnaryOperator::Negate => Self::Negate,
        }
    }
}

fn get_line_number_from_pos(src_code: &str, byte_pos: usize) -> usize {
    lang_c::loc::get_location_for_offset(src_code, byte_pos)
        .0
        .line
}

fn get_line_number(src_code: &str, span: &lang_c::span::Span) -> usize {
    get_line_number_from_pos(src_code, span.start)
}

fn get_function_name(def: &lang_c::ast::FunctionDefinition) -> Option<&str> {
    match &def.declarator.node.kind.node {
        lang_c::ast::DeclaratorKind::Identifier(id) => Some(&id.node.name),
        _ => None,
    }
}

#[serde_as]
#[derive(Serialize, Deserialize, Debug)]
struct FunctionRules {
    #[serde_as(as = "HashSet<DisplayFromStr>")]
    allowed_binary_operators: HashSet<BinaryOperator>,
    #[serde_as(as = "HashSet<DisplayFromStr>")]
    allowed_unary_operators: HashSet<UnaryOperator>,
    allowed_total_ops: usize,
    is_floating_point: bool,
}

#[derive(Debug)]
struct FunctionViolations<'a> {
    rules: &'a FunctionRules,
    src_code: &'a str,
    violations: Vec<String>,
    total_op_count: usize,
}

impl FunctionViolations<'_> {
    fn append_violation(&mut self, message: &str, span: &lang_c::span::Span) {
        let line_number = get_line_number(self.src_code, &span);
        let message = format!("Line {}: {}", line_number, message);
        self.violations.push(message)
    }
}

impl<'ast> lang_c::visit::Visit<'ast> for FunctionViolations<'ast> {
    fn visit_binary_operator(
        &mut self,
        binary_operator: &'ast lang_c::ast::BinaryOperator,
        span: &'ast lang_c::span::Span,
    ) {
        match binary_operator {
            lang_c::ast::BinaryOperator::Assign => (), // Do not count assignment towards operator total
            _ => {
                let converted_op = BinaryOperator::from(binary_operator);
                self.total_op_count += 1;
                if !self.rules.allowed_binary_operators.contains(&converted_op) {
                    let message = format!("Illegal use of operator '{}'", converted_op);
                    self.append_violation(&message, span)
                }
            }
        }

        lang_c::visit::visit_binary_operator(self, binary_operator, span)
    }

    fn visit_unary_operator(
        &mut self,
        unary_operator: &'ast lang_c::ast::UnaryOperator,
        span: &'ast lang_c::span::Span,
    ) {
        self.total_op_count += 1;
        let converted_op = UnaryOperator::from(unary_operator);
        if !self.rules.allowed_unary_operators.contains(&converted_op) {
            let message = format!("Illegal use of operator '{}'", converted_op);
            self.append_violation(&message, span)
        }

        lang_c::visit::visit_unary_operator(self, unary_operator, span)
    }

    fn visit_conditional_expression(
        &mut self,
        conditional_expression: &'ast lang_c::ast::ConditionalExpression,
        span: &'ast lang_c::span::Span,
    ) {
        self.append_violation("Illegal use of '?' operator", span);
        lang_c::visit::visit_conditional_expression(self, conditional_expression, span)
    }

    fn visit_integer(
        &mut self,
        integer: &'ast lang_c::ast::Integer,
        span: &'ast lang_c::span::Span,
    ) {
        let radix: u32;
        match integer.base {
            lang_c::ast::IntegerBase::Octal => {
                radix = 8;
                self.append_violation("Illegal use of octal integer constant", span)
            }
            lang_c::ast::IntegerBase::Binary => {
                radix = 2;
                self.append_violation("Illegal use of binary integer constant", span)
            }
            // Hexadecimal and decimal bases are OK
            lang_c::ast::IntegerBase::Hexadecimal => {
                radix = 16;
            }
            lang_c::ast::IntegerBase::Decimal => {
                radix = 10;
            }
        };

        let i = isize::from_str_radix(&integer.number, radix).expect({
            let line_number = get_line_number(self.src_code, span);
            &format!(
                "Line {}: Failed to parse integer constant '{}'",
                line_number, &integer.number
            )
        });
        if !self.rules.is_floating_point && i > 255 {
            self.append_violation(
                "Illegal use of integer constant greater than 255 (0xFF)",
                span,
            )
        } else if !self.rules.is_floating_point && i < 0 {
            self.append_violation("Illegal use of negative integer constant", span)
        }

        lang_c::visit::visit_integer(self, integer, span)
    }

    fn visit_float(&mut self, float: &'ast lang_c::ast::Float, span: &'ast lang_c::span::Span) {
        self.append_violation("Illegal use of floating-point constant", span);
        lang_c::visit::visit_float(self, float, span)
    }

    fn visit_cast_expression(
        &mut self,
        cast_expression: &'ast lang_c::ast::CastExpression,
        span: &'ast lang_c::span::Span,
    ) {
        self.append_violation("Illegal use of type cast", span);
        lang_c::visit::visit_cast_expression(self, cast_expression, span)
    }

    fn visit_call_expression(
        &mut self,
        call_expression: &'ast lang_c::ast::CallExpression,
        span: &'ast lang_c::span::Span,
    ) {
        self.append_violation("Illegal function call", span);
        lang_c::visit::visit_call_expression(self, call_expression, span)
    }

    fn visit_struct_type(
        &mut self,
        struct_type: &'ast lang_c::ast::StructType,
        span: &'ast lang_c::span::Span,
    ) {
        match struct_type.kind.node {
            lang_c::ast::StructKind::Struct => {
                self.append_violation("Illegal struct declaration", span)
            }
            lang_c::ast::StructKind::Union => {
                self.append_violation("Illegal union declaration", span)
            }
        }

        lang_c::visit::visit_struct_type(self, struct_type, span)
    }

    // We're particularly interested in the declared types of local variables
    // Normally, visiting all type specifiers would be too broad
    // But this should work because we are only traversing specific sub-trees of the larger AST
    // Specifically, only function bodies
    fn visit_type_specifier(
        &mut self,
        type_specifier: &'ast lang_c::ast::TypeSpecifier,
        span: &'ast lang_c::span::Span,
    ) {
        match type_specifier {
            // 'int' is always allowed
            lang_c::ast::TypeSpecifier::Int => (),

            // 'unsigned' is only allowed for floating-point puzzles
            lang_c::ast::TypeSpecifier::Unsigned if self.rules.is_floating_point => (),

            _ => self.append_violation("Illegal data type", span),
        }

        lang_c::visit::visit_type_specifier(self, type_specifier, span)
    }

    fn visit_array_declarator(
        &mut self,
        array_declarator: &'ast lang_c::ast::ArrayDeclarator,
        span: &'ast lang_c::span::Span,
    ) {
        self.append_violation("Illegal use of array", span);
        lang_c::visit::visit_array_declarator(self, array_declarator, span)
    }

    fn visit_statement(
        &mut self,
        statement: &'ast lang_c::ast::Statement,
        span: &'ast lang_c::span::Span,
    ) {
        match statement {
            lang_c::ast::Statement::If(_) => {
                if !self.rules.is_floating_point {
                    self.append_violation("Illegal 'if' statement", span)
                }
            }
            lang_c::ast::Statement::Switch(_) => {
                if !self.rules.is_floating_point {
                    self.append_violation("Illegal 'switch' statement", span)
                }
            }
            lang_c::ast::Statement::While(_) => {
                if !self.rules.is_floating_point {
                    self.append_violation("Illegal 'while' statement", span)
                }
            }
            lang_c::ast::Statement::DoWhile(_) => {
                if !self.rules.is_floating_point {
                    self.append_violation("Illegal 'do-while' statement", span)
                }
            }
            lang_c::ast::Statement::For(_) => {
                if !self.rules.is_floating_point {
                    self.append_violation("Illegal 'for' statement", span)
                }
            }
            lang_c::ast::Statement::Goto(_) => {
                self.append_violation("Illegal 'goto' statement", span)
            }
            _ => (),
        }

        lang_c::visit::visit_statement(self, statement, span)
    }
}

#[derive(Debug)]
enum GlobalCheckResult<'a> {
    InvalidDeclaration(String),
    CheckedFunction(&'a str, FunctionViolations<'a>),
}

impl GlobalCheckResult<'_> {
    fn is_failure(&self) -> bool {
        match self {
            Self::InvalidDeclaration(_) => true,
            Self::CheckedFunction(_, fv) => {
                !fv.violations.is_empty() || (fv.total_op_count > fv.rules.allowed_total_ops)
            }
        }
    }
}

fn check_global_declaration<'a>(
    src_code: &'a str,
    decl: &'a lang_c::ast::ExternalDeclaration,
    rules: &'a HashMap<String, FunctionRules>,
) -> GlobalCheckResult<'a> {
    match decl {
        lang_c::ast::ExternalDeclaration::Declaration(node) => {
            let line_number = get_line_number(src_code, &node.span);
            let message = format!("Line {}: Illegal global declaration", line_number);
            GlobalCheckResult::InvalidDeclaration(message)
        }

        lang_c::ast::ExternalDeclaration::StaticAssert(node) => {
            let line_number = get_line_number(src_code, &node.span);
            let message = format!("Line {}: Illegal static assert", line_number);
            GlobalCheckResult::InvalidDeclaration(message)
        }

        lang_c::ast::ExternalDeclaration::FunctionDefinition(def) => {
            match get_function_name(&def.node) {
                None => {
                    let line_number = get_line_number(src_code, &def.span);
                    let message = format!("Line {}: Illegal Global Declaration", line_number);
                    GlobalCheckResult::InvalidDeclaration(message)
                }

                Some(name) => match rules.get(name) {
                    None => {
                        let line_number = get_line_number(src_code, &def.span);
                        let message = format!(
                            "Line {}: Illegal additional function '{}'",
                            line_number, name
                        );
                        GlobalCheckResult::InvalidDeclaration(message)
                    }

                    Some(rules) => {
                        let mut violations = FunctionViolations {
                            rules: rules,
                            src_code: src_code,
                            violations: Vec::<String>::new(),
                            total_op_count: 0,
                        };
                        violations.visit_function_definition(&def.node, &def.span);
                        GlobalCheckResult::CheckedFunction(name, violations)
                    }
                },
            }
        }
    }
}

fn check_file_globals(
    file_name: &str,
    rules: &HashMap<String, FunctionRules>,
) -> Result<bool, Box<dyn Error>> {
    let config = lang_c::driver::Config::with_gcc();
    let parse = lang_c::driver::parse(&config, file_name)?;

    let function_checks: Vec<GlobalCheckResult> = parse
        .unit
        .0
        .iter()
        .map(|extern_decl| check_global_declaration(&parse.source, &extern_decl.node, rules))
        .filter(|fv| fv.is_failure())
        .collect();

    for check in &function_checks {
        match check {
            GlobalCheckResult::InvalidDeclaration(message) => println!("{}", message),
            GlobalCheckResult::CheckedFunction(name, violations) => {
                println!("== Function '{}'", name);
                violations
                    .violations
                    .iter()
                    .for_each(|msg| println!("{}", msg));
                let total_op_count = violations.total_op_count;
                let allowed_op_count = violations.rules.allowed_total_ops;
                if total_op_count > allowed_op_count {
                    println!(
                        "Total operator count of {} exceeds limit of {}",
                        total_op_count, allowed_op_count
                    );
                }
            }
        }
    }

    Ok(function_checks.is_empty())
}

fn check_preproc_directives(file_name: &str) -> Result<bool, Box<dyn Error>> {
    let include_regex = Regex::new(r"^\s*#include")?;
    let define_regex = Regex::new(r"^\s*#define")?;
    let mut found_violation = false;

    let f = std::fs::File::open(file_name)?;
    let reader = std::io::BufReader::new(f);

    for (num, l) in reader.lines().enumerate() {
        let line = l?;
        if include_regex.is_match(&line) {
            found_violation = true;
            println!("Line {}: Illegal #include directive", num + 1)
        } else if define_regex.is_match(&line) {
            found_violation = true;
            println!("Line {}: Illegal #define directive", num + 1)
        }
    }

    Ok(found_violation)
}

pub fn check_file(spec_file_name: &str, c_file_name: &str) -> Result<bool, Box<dyn Error>> {
    if check_preproc_directives(&c_file_name)? {
        return Ok(false);
    }

    let puzzle_spec = std::fs::read_to_string(spec_file_name)?;
    let function_rules: HashMap<String, FunctionRules> = serde_json::from_str(&puzzle_spec)?;
    check_file_globals(c_file_name, &function_rules)
}

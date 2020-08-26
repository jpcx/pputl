////////////////////////////////////////////////////////////////////////////////
/// @file
//                          __    ___
//                         /\ \__/\_ \
//   _____   _____   __  __\ \ ,_\//\ \
//  /\ '__`\/\ '__`\/\ \/\ \\ \ \/ \ \ \
//  \ \ \_\ \ \ \_\ \ \ \_\ \\ \ \_ \_\ \_
//   \ \ ,__/\ \ ,__/\ \____/ \ \__\/\____\
//    \ \ \   \ \ \   \/___/   \/__/\/____/
//     \/_/    \/_/
//
//  pputl Preprocessor Utilities
//  Copyright (C) 2020 Justin Collier <m@jpcx.dev>
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the internalied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//                                                                            //
//  You should have received a copy of the GNU General Public License        ///
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.  ////
////////////////////////////////////////////////////////////////////////////////

const {spawnSync} = require('child_process');
const fs          = require('fs');

// This script generates a pseudo-recursive reducing macro.
//
// Due to expansion termination of recursive calls, REDUCE may not be chained.
// Instead, use this script to generate a new reducing macro for each use.
//
// see pputl/reduce.h for an example.
//
// USAGE:
//   node genReduceMacro [NAMESPACE] [PREFIX=REDUCE]
//                       [STACK_DEPTH=256] [DETAIL=true]
//
//   NAMESPACE:   macro namespace; prefixed to all macros
//   PREFIX:      unique prefix for current use case
//   STACK_DEPTH: maximum number of arguments to allow
//   DETAIL:      whether or not the main REDUCE macro is detail namespaced
//
// Uses clang-format if it is available.

// -- //

const NAMESPACE   = process.argv[2];
const PREFIX      = process.argv[3] || 'REDUCE';
const STACK_DEPTH = +process.argv[4] || 256;
const DETAIL      = process.argv[5] === 'false' ? false : true;

const BASE16_WIDTH = (STACK_DEPTH - 1).toString(16).length;

function detailname(suffix = '') {
  return [ NAMESPACE, 'DETAIL', PREFIX, suffix.toUpperCase() ]
      .filter(x => x)
      .join('_');
}
function name(suffix = '') {
  if (DETAIL)
    return detailname(suffix);
  return [ NAMESPACE, PREFIX, suffix.toUpperCase() ].filter(x => x).join('_');
}

function reduceStackMacroName(i) {
  return detailname(i.toString(16).padStart(BASE16_WIDTH, '0'));
}

function reduceStackMacro(i) {
  if (i === 0)
    return '#define ' + reduceStackMacroName(i) + '(r, a, is)         a';
  else if (i === 1)
    return '#define ' + reduceStackMacroName(i) +
           `(r, a, is, v)      r(a, v, ${detailname('FIRST')} is)`;
  else
    return '#define ' + reduceStackMacroName(i) +
           `(r, a, is, v, ...) ${reduceStackMacroName(i - 1)}(r, r(a, v, ${
               detailname(
                   'FIRST')} is), (${detailname('REST')} is), __VA_ARGS__)`;
}

function iterators() {
  return Array(STACK_DEPTH).fill(0).map((_, i) => i).join(', ');
}

function reversedBase16ItArray() {
  return Array(STACK_DEPTH)
      .fill('')
      .map(
          (_, i) => (STACK_DEPTH - 1 - i)
                        .toString(16)
                        .padStart(BASE16_WIDTH, '0')
                        .toUpperCase());
}

function choiceParams() {
  return reversedBase16ItArray().slice(0, -1).map(x => '_' + x).join(', ') +
         ', size, ...';
}

function choiceArgs() {
  return '__VA_ARGS__ __VA_OPT__(, ) ' + reversedBase16ItArray().join(', ');
}

// main

let strout = '';

if (!NAMESPACE) {
  console.error(
      'node genReduceMacro [NAMESPACE] [PREFIX=REDUCE] [STACK_DEPTH=256] [DETAIL=true]');
  process.exit();
}

strout += `#define ${name()}(reducer, initial, ...) ${
    detailname('CHOOSER')}(__VA_ARGS__) (reducer, initial, (${
    iterators()}) __VA_OPT__(, ) __VA_ARGS__)\n`;

strout += `#
#define ${detailname('CAT_X')}(a, b) a##b
#define ${detailname('CAT')}(a, b)   ${detailname('CAT_X')}(a, b)
#
#define ${detailname('FIRST')}(i, ...) i
#define ${detailname('REST')}(_, ...) __VA_ARGS__
`;

const blockWidth = reduceStackMacro(STACK_DEPTH - 1).length;

strout += `#/*${Array(blockWidth - 24).join(' ')}*/ // clang-format off\n`;

for (let i = 0; i < STACK_DEPTH; ++i)
  strout += reduceStackMacro(i) + '\n';

strout += `#/*${Array(blockWidth - 24).join(' ')}*/ // clang-format on\n`;

strout += `#define ${detailname('CHOICE')}(${choiceParams()}) ${
    detailname('CAT')}(${detailname() + '_'}, size)
#
#define ${detailname('CHOOSER')}(...) ${detailname('CHOICE')}(${choiceArgs()})
`;

const tmpFileName = '.~' + Date.now();
fs.writeFileSync(tmpFileName, strout, 'utf8');

try {
  const fmtResult = spawnSync('clang-format', [ tmpFileName ])
  if ((fmtResult.stderr + '').trim()) throw new Error();
  console.log((fmtResult.stdout + '').trim());
} catch (_) { console.log(strout) }

fs.unlinkSync(tmpFileName);

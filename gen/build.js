////////////////////////////////////////////////////////////////////////////////
/// @brief documentation builder for jpcx/pputl
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

const fs = require('fs');
const path = require('path');
const {spawnSync} = require('child_process');

// Doxygen

function buildDoxygen() {
  const buildProcess = spawnSync('doxygen', [ 'gen/Doxyfile' ],
                                 {cwd : path.join(__dirname, '../')});
  if ((buildProcess.stderr + '').trim()) {
    console.error('Doxygen errors:')
    console.error(buildProcess.stderr + '');
  }
  console.log('Doxygen output:')
  console.log(buildProcess.stdout + '');
}

// I/O Helpers

function readContent() {
  // {path: content}
  const result = {};

  function recurse(curPath = path.join(__dirname, '../')) {
    for (const child of fs.readdirSync(
             curPath, {encoding : 'utf8', withFileTypes : true})) {
      if (child.isDirectory()) {
        recurse(path.join(curPath, child.name));
      } else if (child.name.match(/\.html$/m)) {
        const p = path.join(curPath, child.name);
        result[p] = fs.readFileSync(p, 'utf8');
      }
    }
  }
  recurse();

  return result;
}

function writeContent(content) {
  for (const [loc, text] of Object.entries(content)) {
    fs.writeFileSync(loc, text, 'utf8');
  }
}

// Manipulation Helpers

function globalSearchReplace(content, searchValue, replaceValue) {
  for (const [loc, text] of Object.entries(content)) {
    content[loc] = text.replace(searchValue, replaceValue)
  }
}

// Fixes

function fixProjectTitle(content) {
  globalSearchReplace(
      content, /<div id="projectname">pputl/g,
      '<div id="projectname"><a href="index.html" style="text-decoration: none">pputl</a>');
  globalSearchReplace(content, /<title>pputl: pputl ([\d.]+).+?<\/title>/g,
                      '<title>pputl $1</title>');
  globalSearchReplace(content, /<div class="title">pputl ([\d.]+)(.+?)<\/div>/g,
                      '<div class="title">pputl $1 | <a href="https://github.com/jpcx/pputl"><em>Repo</em></a>$2</div>');
}

function applyFixes(content) {
  console.log('  Fixing titles...');
  fixProjectTitle(content);
}

// main

console.log('performing initial Doxygen build...');
buildDoxygen();
console.log('reading content...');
const content = readContent();
console.log('applying fixes...');
applyFixes(content);
console.log('writing files...');
writeContent(content);

console.log('done!');

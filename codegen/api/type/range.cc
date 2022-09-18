/* /////////////////////////////////////////////////////////////////////////////
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
//  Copyright (C) 2020 - 2022 Justin Collier <m@jpcx.dev>
//
//	   This program is free software: you can redistribute it and/or modify
//	   it under the terms of the GNU General Public License as published by
//	   the Free Software Foundation, either version 3 of the License, or
//	   (at your option) any later version.
//
//	   This program is distributed in the hope that it will be useful,
//	   but WITHOUT ANY WARRANTY; without even the implied warranty of
//	   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	   GNU General Public License for more details.
//                                                                            //
//  You should have received a copy of the GNU General Public License        ///
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.  ////
///////////////////////////////////////////////////////////////////////////// */

#include "type.h"

namespace api {

using namespace codegen;

decltype(range) range = NIFTY_DEF(range, [&](va args) {
  docs << "[extends object; union tuple|array|stack|queue|heap|map|set]"
       << "constructible from any range type."
       << ""
       << "optionally provide a hint to choose a cast mode."
       << "hints can be calculated using typeof(range)."
       << ""
       << "note: this constructor only performs casts that can safely be performed"
       << "      by rewrapping items without verification. as such, while all ranges"
       << "      are convertible to buffers, any ranges with sorted or structured"
       << "      items (i.e. heap, map, and set) cannot be constructed from others."
       << ""
       << "cast modes:"
       << ""
       << "  tuple  → tuple  | [default]"
       << "  tuple  → array  | requires ARRAY hint"
       << "  tuple  → stack  | requires STACK hint"
       << "  tuple  → queue  | requires QUEUE hint"
       << ""
       << "  array  → array  | [default]"
       << "  array  → tuple  | requires TUPLE hint"
       << "  array  → stack  | requires STACK hint"
       << "  array  → queue  | requires QUEUE hint"
       << ""
       << "  stack  → stack  | [default]"
       << "  stack  → tuple  | requires TUPLE hint"
       << "  stack  → array  | requires ARRAY hint"
       << "  stack  → queue  | requires QUEUE hint"
       << ""
       << "  queue  → queue  | [default]"
       << "  queue  → tuple  | requires TUPLE hint"
       << "  queue  → array  | requires ARRAY hint"
       << "  queue  → stack  | requires STACK hint"
       << ""
       << "  heap   → heap   | [default]"
       << "  heap   → tuple  | requires TUPLE hint"
       << "  heap   → array  | requires ARRAY hint"
       << "  heap   → stack  | requires STACK hint"
       << "  heap   → queue  | requires QUEUE hint"
       << ""
       << "  map    → map    | [default]"
       << "  map    → tuple  | requires TUPLE hint"
       << "  map    → array  | requires ARRAY hint"
       << "  map    → stack  | requires STACK hint"
       << "  map    → queue  | requires QUEUE hint"
       << ""
       << "  set    → set    | [default]"
       << "  set    → tuple  | requires TUPLE hint"
       << "  set    → array  | requires ARRAY hint"
       << "  set    → stack  | requires STACK hint"
       << "  set    → queue  | requires QUEUE hint";

  tests << range() = "()" >> docs;

  // tuple  → tuple  | [default]
  tests << range("(.)")          = "(.)" >> docs;
  // tuple  → array  | requires ARRAY hint
  tests << range("(.)", "ARRAY") = array("(.)") >> docs;
  // tuple  → stack  | requires STACK hint
  tests << range("(.)", "STACK") = stack("(.)") >> docs;
  // tuple  → queue  | requires QUEUE hint
  tests << range("(.)", "QUEUE") = queue("(.)") >> docs;

  // array  → array  | [default]
  tests << range(array("(.)"))          = array("(.)") >> docs;
  // array  → tuple  | requires TUPLE hint
  tests << range(array("(.)"), "TUPLE") = "(.)" >> docs;
  // array  → stack  | requires STACK hint
  tests << range(array("(.)"), "STACK") = stack("(.)") >> docs;
  // array  → queue  | requires QUEUE hint
  tests << range(array("(.)"), "QUEUE") = queue("(.)") >> docs;

  // order  → order  | [default]
  tests << range(order("DESC"))          = order("DESC") >> docs;
  // order  → tuple  | requires TUPLE hint
  tests << range(order("DESC"), "TUPLE") = "()" >> docs;
  // order  → array  | requires ARRAY hint
  tests << range(order("DESC"), "ARRAY") = array() >> docs;
  // order  → stack  | requires STACK hint
  tests << range(order("DESC"), "STACK") = stack() >> docs;
  // order  → queue  | requires QUEUE hint
  tests << range(order("DESC"), "QUEUE") = queue() >> docs;

  // map    → map    | [default]
  tests << range(map())                = map() >> docs;
  // map    → tuple  | requires TUPLE hint
  tests << range(map("FOO_"), "TUPLE") = "()" >> docs;
  // map    → array  | requires ARRAY hint
  tests << range(map("FOO_"), "ARRAY") = array() >> docs;
  // map    → stack  | requires STACK hint
  tests << range(map("FOO_"), "STACK") = stack() >> docs;
  // map    → queue  | requires QUEUE hint
  tests << range(map("FOO_"), "QUEUE") = queue() >> docs;

  // set    → set    | [default]
  tests << range(set())                = set() >> docs;
  // set    → tuple  | requires TUPLE hint
  tests << range(set("FOO_"), "TUPLE") = "()" >> docs;
  // set    → array  | requires ARRAY hint
  tests << range(set("FOO_"), "ARRAY") = array() >> docs;
  // set    → stack  | requires STACK hint
  tests << range(set("FOO_"), "STACK") = stack() >> docs;
  // set    → queue  | requires QUEUE hint
  tests << range(set("FOO_"), "QUEUE") = queue() >> docs;

  // stack  → stack  | [default]
  tests << range(stack("(.)"))          = stack("(.)") >> docs;
  // stack  → tuple  | requires TUPLE hint
  tests << range(stack("(.)"), "TUPLE") = "(.)" >> docs;
  // stack  → array  | requires ARRAY hint
  tests << range(stack("(.)"), "ARRAY") = array("(.)") >> docs;
  // stack  → queue  | requires QUEUE hint
  tests << range(stack("(.)"), "QUEUE") = queue("(.)") >> docs;

  // queue  → queue  | [default]
  tests << range(queue("(.)"))          = queue("(.)") >> docs;
  // queue  → tuple  | requires TUPLE hint
  tests << range(queue("(.)"), "TUPLE") = "(.)" >> docs;
  // queue  → array  | requires ARRAY hint
  tests << range(queue("(.)"), "ARRAY") = array("(.)") >> docs;
  // queue  → stack  | requires STACK hint
  tests << range(queue("(.)"), "STACK") = stack("(.)") >> docs;

  // pqueue → pqueue | [default]
  tests << range(pqueue())                = pqueue() >> docs;
  // pqueue → tuple  | requires TUPLE hint
  tests << range(pqueue("FOO_"), "TUPLE") = "()" >> docs;
  // pqueue → array  | requires ARRAY hint
  tests << range(pqueue("FOO_"), "ARRAY") = array() >> docs;
  // pqueue → stack  | requires STACK hint
  tests << range(pqueue("FOO_"), "STACK") = stack() >> docs;
  // pqueue → queue  | requires QUEUE hint
  tests << range(pqueue("FOO_"), "QUEUE") = queue() >> docs;

  def<"hint_\\TUPLE"> hint_tuple = [&] {
    return "";
  };
  def<"hint_\\ARRAY">{} = [&] {
    return "";
  };
  def<"hint_\\ORDER">{} = [&] {
    return "";
  };
  def<"hint_\\MAP">{} = [&] {
    return "";
  };
  def<"hint_\\SET">{} = [&] {
    return "";
  };
  def<"hint_\\STACK">{} = [&] {
    return "";
  };
  def<"hint_\\QUEUE">{} = [&] {
    return "";
  };
  def<"hint_\\PQUEUE">{} = [&] {
    return "";
  };
  def<"hint_\\AUTO">{} = [&] {
    return "";
  };

  def<"mode(e, t, hint)"> mode = [&](arg e, arg t, arg hint) {
    docs << "cast mode selector and error detector";

    def<"\\TUPLE"> tuple_ = [&] {
      return "";
    };
    def<"\\ARRAY">{} = [&] {
      return "";
    };
    def<"\\ORDER">{} = [&] {
      return "";
    };
    def<"\\MAP">{} = [&] {
      return "";
    };
    def<"\\SET">{} = [&] {
      return "";
    };
    def<"\\STACK">{} = [&] {
      return "";
    };
    def<"\\QUEUE">{} = [&] {
      return "";
    };
    def<"\\PQUEUE">{} = [&] {
      return "";
    };

    def<"\\0(e, t, hint)"> _0 = [&](arg e, arg, arg) {
      return fail(e);
    };
    def<"\\1(e, t, hint)">{} = [&](arg, arg t, arg hint) {
      def<"\\TUPLETUPLE"> tupletuple = [&] {
        return "TU_TU";
      };
      def<"\\TUPLEARRAY">{} = [&] {
        return "TU_AR";
      };
      def<"\\TUPLEORDER">{} = [&] {
        return "FAIL";
      };
      def<"\\TUPLEMAP">{} = [&] {
        return "FAIL";
      };
      def<"\\TUPLESET">{} = [&] {
        return "FAIL";
      };
      def<"\\TUPLESTACK">{} = [&] {
        return "TU_ST";
      };
      def<"\\TUPLEQUEUE">{} = [&] {
        return "TU_QU";
      };
      def<"\\TUPLEPQUEUE">{} = [&] {
        return "FAIL";
      };
      def<"\\TUPLEAUTO">{} = [&] {
        return "TU_TU";
      };
      def<"\\ARRAYTUPLE">{} = [&] {
        return "AR_TU";
      };
      def<"\\ARRAYARRAY">{} = [&] {
        return "AR_AR";
      };
      def<"\\ARRAYORDER">{} = [&] {
        return "FAIL";
      };
      def<"\\ARRAYMAP">{} = [&] {
        return "FAIL";
      };
      def<"\\ARRAYSET">{} = [&] {
        return "FAIL";
      };
      def<"\\ARRAYSTACK">{} = [&] {
        return "AR_ST";
      };
      def<"\\ARRAYQUEUE">{} = [&] {
        return "AR_QU";
      };
      def<"\\ARRAYPQUEUE">{} = [&] {
        return "FAIL";
      };
      def<"\\ARRAYAUTO">{} = [&] {
        return "AR_AR";
      };
      def<"\\ORDERTUPLE">{} = [&] {
        return "OR_TU";
      };
      def<"\\ORDERARRAY">{} = [&] {
        return "OR_AR";
      };
      def<"\\ORDERORDER">{} = [&] {
        return "OR_OR";
      };
      def<"\\ORDERMAP">{} = [&] {
        return "FAIL";
      };
      def<"\\ORDERSET">{} = [&] {
        return "FAIL";
      };
      def<"\\ORDERSTACK">{} = [&] {
        return "OR_ST";
      };
      def<"\\ORDERQUEUE">{} = [&] {
        return "OR_QU";
      };
      def<"\\ORDERPQUEUE">{} = [&] {
        return "FAIL";
      };
      def<"\\ORDERAUTO">{} = [&] {
        return "OR_OR";
      };
      def<"\\MAPTUPLE">{} = [&] {
        return "MA_TU";
      };
      def<"\\MAPARRAY">{} = [&] {
        return "MA_AR";
      };
      def<"\\MAPORDER">{} = [&] {
        return "FAIL";
      };
      def<"\\MAPMAP">{} = [&] {
        return "MA_MA";
      };
      def<"\\MAPSET">{} = [&] {
        return "FAIL";
      };
      def<"\\MAPSTACK">{} = [&] {
        return "MA_ST";
      };
      def<"\\MAPQUEUE">{} = [&] {
        return "MA_QU";
      };
      def<"\\MAPPQUEUE">{} = [&] {
        return "FAIL";
      };
      def<"\\MAPAUTO">{} = [&] {
        return "MA_MA";
      };
      def<"\\SETTUPLE">{} = [&] {
        return "SE_TU";
      };
      def<"\\SETARRAY">{} = [&] {
        return "SE_AR";
      };
      def<"\\SETORDER">{} = [&] {
        return "FAIL";
      };
      def<"\\SETMAP">{} = [&] {
        return "FAIL";
      };
      def<"\\SETSET">{} = [&] {
        return "SE_SE";
      };
      def<"\\SETSTACK">{} = [&] {
        return "SE_ST";
      };
      def<"\\SETQUEUE">{} = [&] {
        return "SE_QU";
      };
      def<"\\SETPQUEUE">{} = [&] {
        return "FAIL";
      };
      def<"\\SETAUTO">{} = [&] {
        return "SE_SE";
      };
      def<"\\STACKTUPLE">{} = [&] {
        return "ST_TU";
      };
      def<"\\STACKARRAY">{} = [&] {
        return "ST_AR";
      };
      def<"\\STACKORDER">{} = [&] {
        return "FAIL";
      };
      def<"\\STACKMAP">{} = [&] {
        return "FAIL";
      };
      def<"\\STACKSET">{} = [&] {
        return "FAIL";
      };
      def<"\\STACKSTACK">{} = [&] {
        return "ST_ST";
      };
      def<"\\STACKQUEUE">{} = [&] {
        return "ST_QU";
      };
      def<"\\STACKPQUEUE">{} = [&] {
        return "FAIL";
      };
      def<"\\STACKAUTO">{} = [&] {
        return "ST_ST";
      };
      def<"\\QUEUETUPLE">{} = [&] {
        return "QU_TU";
      };
      def<"\\QUEUEARRAY">{} = [&] {
        return "QU_AR";
      };
      def<"\\QUEUEORDER">{} = [&] {
        return "FAIL";
      };
      def<"\\QUEUEMAP">{} = [&] {
        return "FAIL";
      };
      def<"\\QUEUESET">{} = [&] {
        return "FAIL";
      };
      def<"\\QUEUESTACK">{} = [&] {
        return "QU_ST";
      };
      def<"\\QUEUEQUEUE">{} = [&] {
        return "QU_QU";
      };
      def<"\\QUEUEPQUEUE">{} = [&] {
        return "FAIL";
      };
      def<"\\QUEUEAUTO">{} = [&] {
        return "QU_QU";
      };
      def<"\\PQUEUETUPLE">{} = [&] {
        return "PQ_TU";
      };
      def<"\\PQUEUEARRAY">{} = [&] {
        return "PQ_AR";
      };
      def<"\\PQUEUEORDER">{} = [&] {
        return "FAIL";
      };
      def<"\\PQUEUEMAP">{} = [&] {
        return "FAIL";
      };
      def<"\\PQUEUESET">{} = [&] {
        return "FAIL";
      };
      def<"\\PQUEUESTACK">{} = [&] {
        return "PQ_ST";
      };
      def<"\\PQUEUEQUEUE">{} = [&] {
        return "PQ_QU";
      };
      def<"\\PQUEUEPQUEUE">{} = [&] {
        return "PQ_PQ";
      };
      def<"\\PQUEUEAUTO">{} = [&] {
        return "PQ_PQ";
      };

      return pp::cat(utl::slice(tupletuple, -10), t, hint);
    };

    return pp::call(xcat(utl::slice(_0, -1), is_none(xcat(utl::slice(tuple_, -5), t))), e,
                    t, hint);
  };

  def<"\\TU_TU(e1, tuple)"> tu_tu = [&](arg, arg tuple) {
    return tuple;
  };
  def<"\\TU_AR(e1, tuple)">{} = [&](arg, arg tuple) {
    return array(tuple);
  };
  def<"\\TU_ST(e1, tuple)">{} = [&](arg, arg tuple) {
    return stack(tuple);
  };
  def<"\\TU_QU(e1, tuple)">{} = [&](arg, arg tuple) {
    return queue(tuple);
  };
  def<"\\AR_TU(e1, array)">{} = [&](arg, arg array) {
    return tuple(array);
  };
  def<"\\AR_AR(e1, array)">{} = [&](arg, arg array) {
    return array;
  };
  def<"\\AR_ST(e1, array)">{} = [&](arg, arg array) {
    return stack(array);
  };
  def<"\\AR_QU(e1, array)">{} = [&](arg, arg array) {
    return queue(array);
  };
  def<"\\OR_TU(e1, order)">{} = [&](arg, arg order) {
    return tuple(order);
  };
  def<"\\OR_AR(e1, order)">{} = [&](arg, arg order) {
    return array(order);
  };
  def<"\\OR_OR(e1, order)">{} = [&](arg, arg order) {
    return order;
  };
  def<"\\OR_ST(e1, order)">{} = [&](arg, arg order) {
    return stack(order);
  };
  def<"\\OR_QU(e1, order)">{} = [&](arg, arg order) {
    return queue(order);
  };
  def<"\\MA_TU(e1, map)">{} = [&](arg, arg map) {
    return tuple(map);
  };
  def<"\\MA_AR(e1, map)">{} = [&](arg, arg map) {
    return array(map);
  };
  def<"\\MA_MA(e1, map)">{} = [&](arg, arg map) {
    return map;
  };
  def<"\\MA_ST(e1, map)">{} = [&](arg, arg map) {
    return stack(map);
  };
  def<"\\MA_QU(e1, map)">{} = [&](arg, arg map) {
    return queue(map);
  };
  def<"\\SE_TU(e1, set)">{} = [&](arg, arg set) {
    return tuple(set);
  };
  def<"\\SE_AR(e1, set)">{} = [&](arg, arg set) {
    return array(set);
  };
  def<"\\SE_SE(e1, set)">{} = [&](arg, arg set) {
    return set;
  };
  def<"\\SE_ST(e1, set)">{} = [&](arg, arg set) {
    return stack(set);
  };
  def<"\\SE_QU(e1, set)">{} = [&](arg, arg set) {
    return queue(set);
  };
  def<"\\ST_TU(e1, stack)">{} = [&](arg, arg stack) {
    return tuple(stack);
  };
  def<"\\ST_AR(e1, stack)">{} = [&](arg, arg stack) {
    return array(stack);
  };
  def<"\\ST_ST(e1, stack)">{} = [&](arg, arg stack) {
    return stack;
  };
  def<"\\ST_QU(e1, stack)">{} = [&](arg, arg stack) {
    return queue(stack);
  };
  def<"\\QU_TU(e1, queue)">{} = [&](arg, arg queue) {
    return tuple(queue);
  };
  def<"\\QU_AR(e1, queue)">{} = [&](arg, arg queue) {
    return array(queue);
  };
  def<"\\QU_ST(e1, queue)">{} = [&](arg, arg queue) {
    return stack(queue);
  };
  def<"\\QU_QU(e1, queue)">{} = [&](arg, arg queue) {
    return queue;
  };
  def<"\\PQ_TU(e1, pqueue)">{} = [&](arg, arg pqueue) {
    return tuple(pqueue);
  };
  def<"\\PQ_AR(e1, pqueue)">{} = [&](arg, arg pqueue) {
    return array(pqueue);
  };
  def<"\\PQ_ST(e1, pqueue)">{} = [&](arg, arg pqueue) {
    return stack(pqueue);
  };
  def<"\\PQ_QU(e1, pqueue)">{} = [&](arg, arg pqueue) {
    return queue(pqueue);
  };
  def<"\\PQ_PQ(e1, pqueue)">{} = [&](arg, arg pqueue) {
    return pqueue;
  };
  def<"\\FAIL(e1, ...)">{} = [&](arg e1, va) {
    return fail(e1);
  };

  return def<"o(e0, e1, v, ...)">{[&](arg e0, arg e1, arg v, va hint) {
    return def<"o(e0, e1, v, ...)">{[&](arg e0, arg e1, arg v, va hint) {
      return pp::call(xcat(utl::slice(tu_tu, -5),
                           mode(e0, typeof(v), enum_(utl::slice(hint_tuple, -5), hint))),
                      e1, v);
    }}(e0, e1, default_("()", v), default_("AUTO", hint));
  }}(error(range, "invalid range", args), error(range, "unsupported cast mode", args),
     args);
});

} // namespace api

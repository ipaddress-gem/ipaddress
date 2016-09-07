#include "mocha.hpp"
#include "chai.hpp"

#include "../src/rle.hpp"

void assert_rle(left: Rle[], right: Rle[]) {
  assert.equal(left.length, right.length, "array length missmatch");
  for (let i = 0; i < left.length; ++i) {
    assert.isOk(left[i].eq(right[i]),
        `left:${left[i].toString()} right:${right[i].toString()}`);
  }
}

int main(int argc, char **argv) {

  describe("rle", []()  {
      it("test_rle", []()  {
          Rle::vec empty;
          assert_rle(Rle.code(empty), []);
          assert_rle(Rle.code(4711), [
              new Rle({
part: 4711,
pos: 0,
cnt: 1,
max: true,
})]);
          assert_rle(Rle.code([4711, 4711]), [
            new Rle({
part: 4711,
pos: 0,
cnt: 2,
max: true,
})]);
          assert_rle(Rle.code([4711, 4711, 4811]), [
            new Rle({
part: 4711,
pos: 0,
cnt: 2,
max: true,
}),
            new Rle({
part: 4811,
pos: 1,
cnt: 1,
max: true,
})]);
assert_rle(Rle.code([4711, 4711, 4811, 4711, 4711]), [
    new Rle({
part: 4711,
pos: 0,
cnt: 2,
max: true,
}),
    new Rle({
part: 4811,
pos: 1,
cnt: 1,
max: true,
}),
    new Rle({
part: 4711,
pos: 2,
cnt: 2,
max: true,
})]);
assert_rle(Rle.code([4711, 4711, 4711, 4811, 4711, 4711]), [
    new Rle({
part: 4711,
pos: 0,
cnt: 3,
max: true,
}),
    new Rle({
part: 4811,
pos: 1,
cnt: 1,
max: true,
}),
    new Rle({
part: 4711,
pos: 2,
cnt: 2,
max: false,
})]
    );
assert_rle(Rle.code([4711, 4711, 4711, 4811, 4711, 4711, 4911, 4911, 4911]),
    [
    new Rle({
part: 4711,
pos: 0,
cnt: 3,
max: true,
}),
    new Rle({
part: 4811,
pos: 1,
cnt: 1,
max: true,
}),
    new Rle({
part: 4711,
pos: 2,
cnt: 2,
max: false,
}),
    new Rle({
part: 4911,
pos: 3,
cnt: 3,
max: true,
})]);


assert_rle(Rle.code([0x2001, 0x888, 0, 0x6630, 0, 0, 0, 0]), [
    new Rle({
part: 0x2001,
pos: 0,
cnt: 1,
max: true,
}),
    new Rle({
part: 0x888,
pos: 1,
cnt: 1,
max: true,
}),
    new Rle({
part: 0,
pos: 2,
cnt: 1,
max: false,
}),
    new Rle({
part: 0x6630,
pos: 3,
cnt: 1,
max: true,
}),
    new Rle({
part: 0,
pos: 4,
cnt: 4,
max: true,
})
]);
});
});
}

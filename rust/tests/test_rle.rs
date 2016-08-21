
extern crate ipaddress;

#[cfg(test)]
mod tests {
    use ipaddress::rle;
    use ipaddress::rle::Rle;
    #[test]
    pub fn test_rle() {
        let empty : Vec<u16> = Vec::new();
        assert_eq!(rle::code(&empty), []);
        assert_eq!(rle::code(&vec![4711]), [Rle {
                        part: 4711,
                        pos: 0,
                        cnt: 1,
                        max: true,
                    }]);
        assert_eq!(rle::code(&vec![4711, 4711]), [Rle {
                        part: 4711,
                        pos: 0,
                        cnt: 2,
                        max: true,
                    }]);
        assert_eq!(rle::code(&vec![4711, 4711, 4811]), [Rle {
                        part: 4711,
                        pos: 0,
                        cnt: 2,
                        max: true,
                    },
                    Rle {
                        part: 4811,
                        pos: 1,
                        cnt: 1,
                        max: true,
                    }]);
        assert_eq!(rle::code(&vec![4711, 4711, 4811, 4711, 4711]), [Rle {
                        part: 4711,
                        pos: 0,
                        cnt: 2,
                        max: true,
                    },
                    Rle {
                        part: 4811,
                        pos: 1,
                        cnt: 1,
                        max: true,
                    },
                    Rle {
                        part: 4711,
                        pos: 2,
                        cnt: 2,
                        max: true,
                    }]);
        assert_eq!(rle::code(&vec![4711, 4711, 4711, 4811, 4711, 4711]), [Rle {
                        part: 4711,
                        pos: 0,
                        cnt: 3,
                        max: true,
                    },
                    Rle {
                        part: 4811,
                        pos: 1,
                        cnt: 1,
                        max: true,
                    },
                    Rle {
                        part: 4711,
                        pos: 2,
                        cnt: 2,
                        max: false,
                    }]
                   );
           assert_eq!(rle::code(&vec![4711, 4711, 4711, 4811, 4711, 4711, 4911, 4911, 4911]), [Rle {
                           part: 4711,
                           pos: 0,
                           cnt: 3,
                           max: true,
                       },
                       Rle {
                           part: 4811,
                           pos: 1,
                           cnt: 1,
                           max: true,
                       },
                       Rle {
                           part: 4711,
                           pos: 2,
                           cnt: 2,
                           max: false,
                       },
                       Rle {
                           part: 4911,
                           pos: 3,
                           cnt: 3,
                           max: true,
                       }]);

    }
}


extern crate ipaddress;

#[cfg(test)]
mod tests {
    use ipaddress::rle;
    use ipaddress::rle::Rle;
    #[test]
    pub fn test_rle() {
        let empty : Vec<u16> = Vec::new();
        assert_eq!(&vec![], rle::code(&empty));
        assert_eq!(&vec![Rle {
                        part: 4711,
                        pos: 0,
                        cnt: 1,
                        max: true,
                    }],
                   rle::code(&vec![4711]));
        assert_eq!(&vec![Rle {
                        part: 4711,
                        pos: 0,
                        cnt: 2,
                        max: true,
                    }],
                   rle::code(&vec![4711, 4711]));
        assert_eq!(&vec![Rle {
                        part: 4711,
                        pos: 0,
                        cnt: 2,
                        max: true,
                    },
                    Rle {
                        part: 4811,
                        pos: 0,
                        cnt: 1,
                        max: true,
                    }],
                   rle::code(&vec![4711, 4711, 4811]));
        assert_eq!(&vec![Rle {
                        part: 4711,
                        pos: 0,
                        cnt: 2,
                        max: false,
                    },
                    Rle {
                        part: 4811,
                        pos: 0,
                        cnt: 1,
                        max: true,
                    },
                    Rle {
                        part: 4711,
                        pos: 0,
                        cnt: 2,
                        max: true,
                    }],
                   rle::code(&vec![4711, 4711, 4811, 4711, 4711]));
        assert_eq!(vec![Rle {
                        part: 4711,
                        pos: 0,
                        cnt: 3,
                        max: true,
                    },
                    Rle {
                        part: 4811,
                        pos: 0,
                        cnt: 1,
                        max: true,
                    },
                    Rle {
                        part: 4711,
                        pos: 0,
                        cnt: 2,
                        max: false,
                    }],
                   rle::code(&vec![4711, 4711, 4711, 4811, 4711, 4711]));
    }
}

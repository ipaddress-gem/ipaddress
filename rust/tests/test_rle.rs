
mod tests {

    #[test]
    pub fn test_rle() {
        assert_eq!([], rle_code([]));
        assert_eq!([Rle{part:4711,0,1,true}], rle_code([4711]));
        assert_eq!([Rle{part:4711,0,2,true}], rle_code([4711,4711]));
        assert_eq!([Rle{part:4711,0,2,true},Rle{part:4811,0,1,true}], rle_code([4711,4711,4811]));
        assert_eq!([Rle{part:4711,0,2,false},
                    Rle{part:4811,0,1,true},
                    Rle{part:4711,0,2,true}], rle_code([4711,4711,4811,4711,4711]));
        assert_eq!([Rle{part:4711,0,3,true},
                    Rle{part:4811,0,1,true},
                    Rle{part:4711,0,2,false}], rle_code([4711,4711,4711,4811,4711,4711]));
    }
}

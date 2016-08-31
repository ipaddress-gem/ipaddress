declare module 'number-crunch' {

  export function parse(value: string): number[];
  export function compare(n1: number[], n2: number[]): number;
  export function leftShift(nums: number[], num: number): number[];
  export function rightShift(nums: number[], num: number): number[];
  export function add(n1: number[], n2: number[]): number[];
  export function mul(n1: number[], n2: number[]): number[];
  export function sub(n1: number[], n2: number[]): number[];
  export function mod(n1: number[], n2: number[]): number[];
  export function div(n1: number[], n2: number[]): number[];

}

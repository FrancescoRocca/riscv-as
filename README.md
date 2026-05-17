# riscv-as

A tiny and simple RISC-V assembler.

## Building

```bash
$ meson setup build
$ cd build
$ meson compile
```

## Testing

If you would like to test the assembled file, you can do it using [spike](https://github.com/riscv-software-src/riscv-isa-sim) and [pk](https://github.com/riscv-software-src/riscv-pk).

**Note**: compile spike and pk to support 32 bits versions. They require the [riscv-gnu-toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain) with multilib enabled.

## Contributing

Remember to format the code before making any contribute. Just run `ninja format` inside the `build/` folder.

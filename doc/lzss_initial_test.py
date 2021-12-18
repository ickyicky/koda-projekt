import io


def find_longest_match(dictionary, input_buffer):
    found, length, position = 1, 0, 0

    for i in range(len(dictionary)):
        l = 0

        for j in range(len(dictionary) - i):
            if dictionary[i + j] != input_buffer[j]:
                break

            l += 1

        if l > length:
            length = l
            found = 0
            position = i

    return (found, position, length)


def compress(input_stream, n, k, output_stream):
    input_buffer = input_stream.read(k)
    dictionary = n * bytes(input_buffer[:1])
    output_stream.write(bytes(input_buffer[:1]))

    while True:
        found, position, length = find_longest_match(dictionary, input_buffer)
        if found == 0:
            output_stream.write(bytes([found, position, length]))
        else:
            output_stream.write(bytes([found, input_buffer[0]]))
            length = 1

        dictionary = dictionary[length:] + input_buffer[:length]
        input_buffer = input_buffer[length:] + input_stream.read(length)

        if len(input_buffer) == 0:
            break


def decompress(input_stream, n, k, output_stream):
    dictionary = n * input_stream.read(1)

    while True:
        found = input_stream.read(1)
        if not found:
            break

        found = found[0]

        length = 1
        output = None

        if found == 0:
            position = input_stream.read(1)[0]
            length = input_stream.read(1)[0]
            output = dictionary[position:position + length]
        else:
            output = input_stream.read(1)

        output_stream.write(output)
        dictionary = dictionary[length:] + output


test_in = io.BytesIO(b"aabbcabbcabd")
test_out = io.BytesIO()

compress(test_in, 4, 4, test_out)
test_out.seek(0)

test_out.seek(0)
test_decompress = io.BytesIO()

decompress(test_out, 4, 4, test_decompress)

test_in.seek(0)
test_out.seek(0)
test_decompress.seek(0)


print(test_in.read())
print(test_out.read())
print(test_decompress.read())
test_in.close()
test_out.close()
test_decompress.close()

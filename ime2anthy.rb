#!/usr/bin/env ruby
# coding: utf-8

require 'pp'
require 'nkf'

DICT_FILE_NAME = ENV["HOME"] + "/.anthy/2ch_kaomoji.txt"

def load_dict_file(flg)
    cmd = ""

    if !flg then
	    cmd = sprintf "cat %s | anthy-dic-tool --load --utf8", DICT_FILE_NAME
    else
	    cmd = sprintf "cat %s | anthy-dic-tool --load --utf8 --append", DICT_FILE_NAME
    end
	system(cmd)
end

def create_dict_data(text)
	tmp = text.split(/\s*\t\s*/)
    if tmp.size == 3
        if tmp[0].index("ヴ") != nil then 
            tmp[0] = tmp[0].gsub("ヴ", "ぼ")
        elsif tmp[0] == "・・・" then
            tmp[0] = "てんてんてん"
        end
    end

    return tmp 
end

def write_dict_file(data)
	f = open(DICT_FILE_NAME, "w")

	data.each { |d|
        arr = create_dict_data(d)
		s = sprintf "%s 1 %s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
			NKF::nkf( '-WwXm0', arr[0]),
			arr[1],
			"品詞 = 名詞",
			"な接続 = y",
			"さ接続 = y",
			"する接続 = y",
			"語幹のみで文節 = y",
			"格助詞接続 = y"
		f.write(s)
	}
	f.close
end

def convert_sjis_to_utf8(name)
	arr = []
	
	f = open(name) { |file|
		while line = file.gets
			arr.push(NKF::nkf( '-Swm0', line))
		end
	}
	return arr
end

def check_file(name)
	return File.exist?(name)
end

if __FILE__ == $0
    if ARGV.size == 0 then 
        printf "usage: %s [dict file]\n", $0
        exit 0
    end
    f = ARGV[0]

    append_flg = false
    if ARGV.size == 2 then
        append_flg = true
    end

	if ! check_file(f) then
		printf "file %s is not found\n", f
		exit(0)
	end
	
	data = convert_sjis_to_utf8(f)
	
	write_dict_file(data)
	
	load_dict_file(append_flg)
	
	printf "Done.\n"
end

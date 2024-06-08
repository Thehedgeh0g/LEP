#include <fstream>
#include <vector>
#include <valarray>
/*
 * 16.4. Сугробы на ЛЭП (8)
Служба электроснабжения проводит мониторинг уровня снега, лежащего на ЛЭП.
Вся ЛЭП разбивается на участки опорами. Если снег падает на некоторый интервал ЛЭП,
 то высота снежного покрова на этом интервале увеличивается на высоту выпавшего снега.
 Снег также имеет тенденцию таять на некотором участке трассы в результате оттепели, однако
 сугробов отрицательной толщины быть не может. Энергетикам крайне важно уметь узнавать суммарную
 высоту снежного покрова на некоторых участках, чтобы определять вероятность обрыва проводов.
Ввод из файла INPUT.TXT. В первой строке находятся через пробел два числа: N – количество опор и M – количество команд (1 ≤ N, M ≤ 105). Команды бывают двух видов:
1 L R S -  на участок с L-й опоры по R-ю выпало S сантиметров снега (-100 ≤ S ≤ 100);
2 L R – запрос о высоте снега на участке от L-й опоры по R-ю (1 ≤ L < R ≤ N).
Таяние снега показывает первый вид команды с отрицательным значением S. Опоры нумеруются от 1 до N.
Вывод в файл OUTPUT.TXT. На каждый запрос (команду второго вида) требуется выводить суммарную высоту снежного покрова, лежащего на проводах от L-й опоры по R-ю.
Пример
Ввод
11 5
1 1 10 10
1 2 6 -3
2 5 9
1 1 7 25
2 1 3
Вывод
37
67
Лежнин И.3. ПС-23
 */


const std::string INPUT_FILE_NAME = "../input5.txt";
const std::string OUTPUT_FILE_NAME = "../output.txt";

std::ifstream getFileInput()
{
    std::ifstream fileInput(INPUT_FILE_NAME);
    if (!fileInput.is_open())
    {
        throw std::runtime_error("INPUT file not found");
    }
    return fileInput;
}

std::ofstream getFileOutput()
{
    std::ofstream fileOutputClear(OUTPUT_FILE_NAME, std::ios::trunc);
    if (!fileOutputClear.is_open())
    {
        throw std::runtime_error("OUTPUT file not found");
    }
    fileOutputClear.close();

    std::ofstream fileOutput(OUTPUT_FILE_NAME, std::ios::app);
    if (!fileOutput.is_open())
    {
        throw std::runtime_error("Error to open first OUTPUT file");
    }
    return fileOutput;
}

struct segtree {
    long long lb, rb;
    long long sum = 0;
    long long lazy = 0;
    segtree *l = 0, *r = 0;

    segtree(long long _lb, long long _rb) {
        lb = _lb, rb = _rb;
        if (lb + 1 < rb) {
            long long t = (lb + rb) / 2;
            l = new segtree(lb, t);
            r = new segtree(t, rb);
        }
    }

    void apply(long long x) {
        sum += (rb - lb) * x;
        if (l) {
            lazy += x;
        }
    }

    void push() {
        if (lazy != 0) {
            l->apply(lazy);
            r->apply(lazy);
            lazy = 0;
        }
    }

    void add_range(long long lq, long long rq, long long x) {
        if (lb >= lq && rb <= rq) {
            apply(x);
            return;
        }
        if (std::max(lb, lq) >= std::min(rb, rq))
            return;
        push();
        l->add_range(lq, rq, x);
        r->add_range(lq, rq, x);
        sum = l->sum + r->sum;
    }

    long long get_sum(long long lq, long long rq) {
        if (lb >= lq && rb <= rq)
            return sum;
        if (std::max(lb, lq) >= std::min(rb, rq))
            return 0;
        push();
        return l->get_sum(lq, rq) + r->get_sum(lq, rq);
    }
};


int main()
{
    std::ifstream fileInput = getFileInput();
    std::ofstream fileOutput = getFileOutput();
    long long n, m, type, l, r, val;
    fileInput >> n >> m;
    std::vector<long long> towers(n, 0);
    segtree treeA(0, n);
    for (long long i = 0; i < m; ++i)
    {
        fileInput >> type;
        if (type == 1)
        {
            fileInput >> l >> r >> val;
            treeA.add_range(l, r, val);
        }
        if (type == 2)
        {
            fileInput >> l >> r;
            fileOutput << treeA.get_sum(l, r) << std::endl;
        }
    }
    return 0;
}

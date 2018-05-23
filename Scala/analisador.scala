import org.apache.spark.SparkContext
import org.apache.spark.SparkContext._
import org.apache.spark.SparkConf


object Analisador {

  
  def main(args: Array[String]) {
    val bgin = System.currentTimeMillis()
    // criar contexto Spark
    val sc = new SparkContext(new SparkConf().setAppName("Contagem de Palavra"))

    println("TEXT1")

    // ler o primeiro texto
    val lines1 = sc.textFile(args(0))
   
    // contar palavras do texto 1 e imprimir as 5 palavras com as maiores ocorrencias (ordem DECRESCENTE)
    // imprimir na cada linha: "palavra=numero"

    val count1 = lines1.flatMap(line => line.split(" "))
    val mapred1 = count1.map(word => (word.replaceAll("[,.!?:;]","").toLowerCase(),1)).reduceByKey(_+_)
    val s1 = mapred1.sortBy(_._2,false)
    val removed1 = s1.filter (line => line._1.length > 3)
    removed1.collect( )
    removed1.take(5).foreach(line=>println(line._1+"="+line._2))
    
    val most1 = s1.filter (line => line._2 > 100 && line._1.length > 3 )

    println("TEXT2")

    // ler o segundo texto
    val lines2 = sc.textFile(args(1))

    // contar palavras do texto 2 e imprimir as 5 palavras com as maiores ocorrencias (ordem DECRESCENTE)
    // imprimir na cada linha: "palavra=numero"
    
    val count2 = lines2.flatMap(line => line.split(" "))
    val mapred2 = count2.map(word => (word.replaceAll("[,.!?:;]","").toLowerCase(),1)).reduceByKey(_+_)
    val s2 = mapred2.sortBy(_._2,false)
    val removed2 = s2.filter (line => line._1.length > 3)
    removed2.collect( )
    removed2.take(5).foreach(line=>println(line._1+"="+line._2 ))

    val most2 = s2.filter (line => line._2 > 100 && line._1.length > 3)

    println("COMMON")

    // comparar resultado e imprimir na ordem ALFABETICA todas as palavras que aparecem MAIS que 100 vezes nos 2 textos
    // imprimir na cada linha: "palavra"

    val group = most1.cogroup(most2)
    val filtRdd = group.filter { case (x, (v1, v2)) => v1.nonEmpty && v2.nonEmpty }
     
    //agrupar as duas estruturas criadas e fazer a intersecao
    filtRdd.sortBy(x => x._1).collect().foreach(x =>println(x._1 ))
    val end = System.currentTimeMillis()
    val time = end - bgin
    println (time)
  }

}
